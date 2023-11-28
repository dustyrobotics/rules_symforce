/* Copyright (C) Basemap, Inc DBA Automaton  All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Asa Hammond <asa@automaton.is>, 2021
 */
#define CATCH_CONFIG_MAIN
#include "common/imsym/imsym.hh"

#include <spdlog/spdlog.h>
#include <sym/factors/between_factor_pose3.h>
#include <sym/factors/between_factor_rot3.h>
#include <sym/factors/prior_factor_pose3.h>
#include <sym/factors/prior_factor_pose3_position.h>
#include <sym/factors/prior_factor_rot3.h>
#include <sym/pose3.h>
#include <symforce/opt/factor.h>
#include <symforce/opt/key.h>
#include <symforce/opt/optimizer.h>
#include <symforce/opt/util.h>
#include <symforce/opt/values.h>
#include <catch2/catch_all.hpp>
#include <immer/array.hpp>
#include <sym/index_entry_t.hpp>

using Catch::Matchers::WithinAbs;
constexpr double tol = 1e-10;

#include <chrono>

using sym::Pose3d;
using sym::Rot3d;
using sym::Vector3d;

auto CHECK_KEY = [](auto key, auto sym_key) {
    CHECK(key.letter == sym_key.Letter());
    CHECK(key.sub == sym_key.Sub());
    CHECK(key.super == sym_key.Super());
};

auto DefaultLmParams() -> sym::optimizer_params_t {
    sym::optimizer_params_t params{};
    params.iterations = 50;
    params.verbose = true;
    params.initial_lambda = 1.0;
    params.lambda_up_factor = 4.0;
    params.lambda_down_factor = 1 / 4.0;
    params.lambda_lower_bound = 0.0;
    params.lambda_upper_bound = 1000000.0;
    params.early_exit_min_reduction = 0.0001;
    params.use_unit_damping = true;
    params.use_diagonal_damping = false;
    params.keep_max_diagonal_damping = false;
    params.diagonal_damping_min = 1e-6;
    params.enable_bold_updates = false;
    return params;
}

auto CHECK_DATA_EQUAL = [](auto values, auto sym_values) {
    for (size_t i = 0; i < values.data.size(); i++) {
        CHECK(values.data.at(i) == sym_values.Data().at(i));
    }
};

auto CHECK_VALUES_EQUAL = [](auto values, auto sym_values) {
    for (const auto& k : sym_values.Keys()) {
        /*
         * TODO use each entry type to automatically pull out its type
         const auto a = at(values, to(k));
         CHECK(a == sym_values.at<std::decay(decltype(a))::type>(k));
         */
    }
};

auto CHECK_EQUAL = [](auto values, auto sym_values) {
    CHECK_VALUES_EQUAL(values, sym_values);

    CHECK(values.map.size() == sym_values.NumEntries());
    CHECK(values.map.size() == sym_values.Items().size());
    CHECK(values.data.size() == sym_values.Data().size());
};

TEST_CASE("conversion between immutable and base symforce types") {
    SECTION("keys") {
        char letter = 'P';
        SECTION("letter") {
            auto key = imsym::key::key_t{.letter = letter};
            auto sym_key = sym::Key(letter);
            CHECK_KEY(key, sym_key);
            CHECK(key.sub == imsym::key::key_t::kInvalidSub);
            CHECK(key.super == imsym::key::key_t::kInvalidSuper);
        };
        SECTION("letter, sub") {
            int64_t sub = 12345;
            auto key = imsym::key::key_t{.letter = letter, .sub = sub};
            auto sym_key = sym::Key(letter, sub);
            CHECK_KEY(key, sym_key);
            CHECK(key.super == imsym::key::key_t::kInvalidSuper);
        };
        SECTION("letter, sub, super") {
            int64_t sub = 12345;
            int64_t super = 9999;
            auto key = imsym::key::key_t{.letter = letter, .sub = sub, .super = super};
            auto sym_key = sym::Key(letter, sub, super);
            CHECK_KEY(key, sym_key);
        };
    };

    SECTION("index entry type") {
        auto entry = imsym::values::index_entry_t{
            .key = imsym::key::key_t{.letter = 'P', .sub = 0},
            .type = sym::type_t{},  // enum entry for different types, storageops fromStorage loads
                                    // from scalar* data pointer
            .offset = 100,
            .storage_dim = 3,
            .tangent_dim = 3,
        };

        auto sym_entry = sym::index_entry_t{};
        // CHECK_KEY(entry.key, sym_entry.key);
    };
    SECTION("values") {
        using Scalar = double;
        auto sym_key_0 = sym::Key('P', 0);
        auto sym_pose_0 = Pose3d(Rot3d::FromQuaternion({0, 1, 2, 3}), Vector3d{4, 5, 6});
        SECTION("set") {
            auto values = imsym::values::valuesd_t{};

            values = imsym::values::set<Scalar>(values, imsym::key::to(sym_key_0), sym_pose_0);
            auto sym_values = sym::Valuesd{};
            sym_values.Set(sym_key_0, sym_pose_0);

            CHECK_EQUAL(values, sym_values);

            SECTION("at") {
                imsym::values::index_entry_t entry = values.map.at(imsym::key::to(sym_key_0));
                const Pose3d a = imsym::values::at<Pose3d>(values, entry);
                CHECK(a == sym_values.At<std::decay<decltype(a)>::type>(sym_key_0));
            }

            SECTION("set, remove, set, should have a gap") {
                auto sym_key_1 = sym::Key('P', 1);
                auto sym_pose_1 = Pose3d(Rot3d::FromQuaternion({7, 8, 9, 10}), Vector3d{11, 12, 13});
                values = imsym::values::set<Scalar>(values, imsym::key::to(sym_key_1), sym_pose_1);
                sym_values.Set(sym_key_1, sym_pose_1);

                /*
                auto sym_rot_key_1 = sym::Key('R', 1);
                auto sym_rot_1 = Rot3d::FromQuaternion({7, 8, 9, 10});

                values =
                    imsym::values::set<Scalar>(values, imsym::key::to(sym_rot_key_1), sym_rot_1);
                sym_values.Set(sym_rot_key_1, sym_rot_1);
                */

                CHECK_EQUAL(values, sym_values);

                SECTION("remove a value") {
                    sym_values.Remove(sym_key_1);
                    values = imsym::values::remove<Scalar>(values, imsym::key::to(sym_key_1));
                    CHECK_EQUAL(values, sym_values);
                    SECTION("add another value") {
                        auto sym_key_2 = sym::Key('P', 2);
                        auto sym_pose_2 = Pose3d(Rot3d::FromQuaternion({14, 15, 16, 17}), Vector3d{18, 19, 20});

                        values = imsym::values::set<Scalar>(values, imsym::key::to(sym_key_2), sym_pose_2);
                        sym_values.Set(sym_key_2, sym_pose_2);

                        CHECK_EQUAL(values, sym_values);

                        SECTION("then cleanup") {
                            sym_values.Cleanup();
                            auto [new_values, new_size] = imsym::values::cleanup<Scalar>(values);
                            values = new_values;
                            for (const auto& v : values.data) {
                                // the raw values in the array
                                spdlog::error("imsym: {}", v);
                            }
                            for (const auto& v : sym_values.Data()) {
                                // the raw values in the array
                                spdlog::error("sym: {}", v);
                            }

                            // only holds two pose3s worth of data now
                            CHECK(values.data.size() == sym::StorageOps<Pose3d>::StorageDim() * 2);
                            CHECK_EQUAL(values, sym_values);
                            CHECK_VALUES_EQUAL(values, sym_values);
                            spdlog::error("values: {} ", sym_values);
                            spdlog::error("imsym values:");

                            for (const auto& [k, v] : values.map) {
                                spdlog::error("{} {}", k, v);
                            }
                            // note the order is not preserved between these implimentations

                            SECTION("clone sym::Values") {
                                const auto imsym_cloned_values = imsym::values::clone(sym_values);

                                CHECK_DATA_EQUAL(imsym_cloned_values, sym_values);
                                CHECK_VALUES_EQUAL(imsym_cloned_values, sym_values);

                                SECTION("clone imsym::values_t") {
                                    auto cloned_sym_values = imsym::values::clone(imsym_cloned_values);

                                    CHECK_DATA_EQUAL(imsym_cloned_values, sym_values);
                                    CHECK_VALUES_EQUAL(imsym_cloned_values, sym_values);

                                    SECTION("optimize with a cloned version") {
                                        const double epsilon = 1e-10;
                                        const int num_keys = 10;

                                        sym::Valuesd values;

                                        // Costs
                                        const double prior_start_sigma = 0.1;  // [rad]
                                        const double prior_last_sigma = 0.1;   // [rad]
                                        const double between_sigma = 1.0;      // [rad]

                                        // Set points (doing 180 rotation and 5m translation to keep
                                        // it hard)
                                        const sym::Pose3d prior_start = sym::Pose3d(
                                            sym::Rot3d::FromYawPitchRoll(0.0, 0.0, 0.0), Eigen::Vector3d::Zero());

                                        const sym::Pose3d prior_last = sym::Pose3d(
                                            sym::Rot3d::FromYawPitchRoll(M_PI, 0.0, 0.0), Eigen::Vector3d(5, 0, 0));

                                        const auto create_prior_factor = [&epsilon](const sym::Key& key,
                                                                                    const sym::Pose3d& prior,
                                                                                    const double sigma) {
                                            return sym::Factord::Jacobian(
                                                [&prior, sigma, &epsilon](const sym::Pose3d& pose,
                                                                          sym::Vector6d* const res,
                                                                          sym::Matrix66d* const jac) {
                                                    const sym::Matrix66d sqrt_info =
                                                        sym::Vector6d::Constant(1 / sigma).asDiagonal();

                                                    sym::PriorFactorPose3<double>(pose, prior, sqrt_info, epsilon, res,
                                                                                  jac);
                                                },
                                                {key});
                                        };

                                        // Add priors
                                        std::vector<sym::Factord> factors;
                                        factors.push_back(
                                            create_prior_factor({'P', 0}, prior_start, prior_start_sigma));
                                        factors.push_back(
                                            create_prior_factor({'P', num_keys - 1}, prior_last, prior_last_sigma));

                                        const auto create_position_prior_factor = [&epsilon](const sym::Key& key,
                                                                                             const sym::Vector3d& prior,
                                                                                             const double sigma) {
                                            return sym::Factord::Jacobian(
                                                [&prior, sigma, &epsilon](const sym::Pose3d& pose,
                                                                          sym::Vector3d* const res,
                                                                          sym::Matrix36d* const jac) {
                                                    const sym::Matrix33d sqrt_info =
                                                        sym::Vector3d::Constant(1 / sigma).asDiagonal();

                                                    sym::PriorFactorPose3Position<double>(pose, prior, sqrt_info,
                                                                                          epsilon, res, jac);
                                                },
                                                {key});
                                        };

                                        // push the signal off to the left a bit
                                        factors.push_back(create_position_prior_factor(
                                            {'P', num_keys / 2}, Eigen::Vector3d{2.5, 1.0, 0.0}, prior_last_sigma));

                                        // Add between factors in a chain
                                        for (int i = 0; i < num_keys - 1; ++i) {
                                            factors.push_back(sym::Factord::Jacobian(
                                                [&between_sigma, &epsilon](const sym::Pose3d& a, const sym::Pose3d& b,
                                                                           sym::Vector6d* const res,
                                                                           Eigen::Matrix<double, 6, 12>* const jac) {
                                                    const sym::Matrix66d sqrt_info =
                                                        sym::Vector6d::Constant(1 / between_sigma).asDiagonal();

                                                    const sym::Pose3d a_T_b = sym::Pose3d::Identity();

                                                    sym::BetweenFactorPose3<double>(a, b, a_T_b, sqrt_info, epsilon,
                                                                                    res, jac);
                                                },
                                                /* keys */ {{'P', i}, {'P', i + 1}}));
                                        }

                                        // Create initial values as random pertubations from the
                                        // first prior
                                        std::mt19937 gen(42);
                                        for (int i = 0; i < num_keys; ++i) {
                                            const sym::Pose3d value =
                                                prior_start.Retract(0.4 * sym::Random<sym::Vector6d>(gen));
                                            values.Set<sym::Pose3d>({'P', i}, value);
                                        }

                                        spdlog::info("Initial values: {}", values);
                                        spdlog::info("Prior on P0: {}", prior_start);
                                        spdlog::info("Prior on P[-1]: {}", prior_last);

                                        // Optimize
                                        sym::optimizer_params_t params = DefaultLmParams();
                                        params.iterations = 50;
                                        params.early_exit_min_reduction = 0.0001;

                                        sym::Optimizer<double> optimizer(params, factors, epsilon, "sym::Optimize", {},
                                                                         /* debug_stats */ false,
                                                                         /* check_derivatives */ true,
                                                                         /* include_jacobians */ true);

                                        // round trip through imsym
                                        auto imsym_values = imsym::values::clone(values);
                                        auto sym_values = imsym::values::clone(imsym_values);

                                        const auto stats = optimizer.Optimize(sym_values);
                                        spdlog::error("optimized sym values: {}", sym_values);
                                    }
                                }
                            }
                        }
                    }
                }
            };
        };
        SECTION("compact"){
            // a hole in the data should be removed

        };

        SECTION("initializer list") {
            // a hole in the data should be removed

            imsym::values::valuesd_t values{{
                {
                    {'a'},
                    1.0,
                },
                {{'b'}, sym::Vector3d{1, 2, 3}},
            }};

            CHECK(has(values, {'a'}));
            CHECK(has(values, {'b'}));
        };
        SECTION("roundtrip") {
            SECTION("from immutable "){};
            SECTION("to immutable"){};
        };
    };
    SECTION("factors"){
        // each factor should be typed with a struct, and serialied with COMMON_STRUCT
        // a variant of all factor types should be operated over when adding factors to the system
        // factors should be hashable
    };
};

/*
auto diff(const immer::flex_vector& vec_a,
          const immer::flex_vector& vec_b,
          auto&& AddedFn,
          auto&& RemovedFn,
          auto&& ChangedFn) {
    if (vec_a == vec_b) {
        return;
    }
    if (vec_a.size() == 0) {
        AddedFn(vec_b);
        return;
    }

    for (const auto& a : vec_a) {
    }
};
*/
using namespace imsym::values;

/*
inline auto diff(const valuesd_t& va,
                 const valuesd_t& vb,
                 auto&& KeyAddedFn,
                 auto&& KeyRemovedFn,
                 auto&& DataChangedFn) {
    // make an index of a and an index of b
    // for each key in a, check key exists in b and that the data is the same
    // if data is different call the ChangedFn
    // if the key is not in b, call the RemovedFn
    // if the key is only in b, call the AddedFn

    // handle keys through the map diff

    auto key_added = [](auto&& x) {
        spdlog::info("added {}", x);
    };

    auto key_removed = [](auto&& x) {
        spdlog::info("removed {}", x);
    };

    auto key_changed = [](auto&& x, auto&& y) {
        spdlog::info("changed {} to {} ", x, y);
    };

    immer::diff(va.map, vb.map, key_added, key_removed, key_changed);

    const auto keys_a = keys<>(va);
    const auto full_index_a = create_index(va, keys_a);
    for (const auto& entry : full_index_a.entries) {
        seen = move(seen).insert(entry.key);

        if (not has(vb, entry.key)) {
            RemovedFn(entry);
            continue;
        }
    }
};
*/

TEST_CASE("check diff operations") {
    auto sym_key_0 = sym::Key('P', 0);
    auto sym_pose_0 = Pose3d(Rot3d::FromQuaternion({0, 1, 2, 3}), Vector3d{4, 5, 6});
    auto sym_key_1 = sym::Key('P', 1);
    auto sym_pose_1 = Pose3d(Rot3d::FromQuaternion({10, 11, 12, 13}), Vector3d{14, 15, 16});
    auto sym_pose_1_alt = Pose3d(Rot3d::FromQuaternion({20, 21, 22, 23}), Vector3d{24, 25, 26});

    auto sym_config_key_0 = sym::Key('c', 0);
    auto sym_config_0 = sym::Vector3d{111, 222, 333};

    auto sym_measurement_key_0 = sym::Key('m', 0);
    auto sym_measurement_0 = sym::Vector3d{444, 555, 666};

    auto sym_measurement_key_1 = sym::Key('m', 1);
    auto sym_measurement_1 = sym::Vector3d{777, 888, 999};

    auto values = imsym::values::valuesd_t{};

    /*
    WHEN("diff") {
        values = imsym::values::set<>(values, imsym::key::to(sym_key_0), sym_pose_0);
        auto values2 = imsym::values::set<>(values, imsym::key::to(sym_key_1), sym_pose_1);

        auto map_diff = [](auto a, auto b, auto added, auto removed, auto changed) {
            // see what keys are different
            immer::diff(a.map, b.map, added, removed, changed);
        };

        auto data_diff = [](auto a, auto b, auto added, auto removed, auto changed) {
            immer::diff(a.data, b.data, added, removed, changed);
        };

        auto added = [](auto&& x) {
            spdlog::info("added {}", x);
        };

        auto removed = [](auto&& x) {
            spdlog::info("removed {}", x);
        };

        auto changed = [](auto&& x, auto&& y) {
            spdlog::info("changed {} to {} ", x, y);
        };

        map_diff(values, values2, added, removed, changed);

        // update a value
        WHEN("change a value at a key") {
            bool data_value_updated = false;
            bool map_value_updated = false;

            auto map_changed = [&](auto&& x, auto&& y) {
                map_value_updated = true;
            };
            auto data_changed = [&](auto&& x, auto&& y) {
                data_value_updated = true;
            };
            auto values3 = imsym::values::set<>(values, imsym::key::to(sym_key_1), sym_pose_1_alt);
            map_diff(values2, values3, added, removed, map_changed);
            data_diff(values2, values3, added, removed, data_changed);
            CHECK(not map_value_updated);   // index doesn't change, just the data
            CHECK(data_value_updated);
        }

        WHEN("whole values obj is roundtriped through sym::Values") {
            // no data changed
            // no keys changed
            auto round_tripped = imsym::values::clone(imsym::values::clone(values2));
            map_diff(values2, round_tripped, added, removed, changed);
            map_diff(values, round_tripped, added, removed, changed);
        }
    }

    */
    WHEN("merge") {
        auto values = imsym::values::valuesd_t{{imsym::key::to(sym_key_0), sym_pose_0}};
        auto values2 = imsym::values::valuesd_t{{imsym::key::to(sym_key_1), sym_pose_1}};

        SECTION("non overlapping values") {
            spdlog::error("original values {}", values);
            spdlog::error("original values2 {}", values2);
            auto values3 = merge(values, values2);
            spdlog::error("merged values {}", values3);

            auto correct = imsym::values::valuesd_t{
                {imsym::key::to(sym_key_0), sym_pose_0},
                {imsym::key::to(sym_key_1), sym_pose_1},
            };
            spdlog::error("correct.map {}", correct.map);
            spdlog::error("values3.map {}", values3.map);
            CHECK(values3.map == correct.map);
            CHECK(values3.data == correct.data);
            CHECK(contents_equal(values3, correct));
        }

        /*
        SECTION("overlapping, some keys are shared") {
            auto values3 = imsym::values::set<>(values, imsym::key::to(sym_key_1), sym_pose_1_alt);
            auto values4 = merge(values2, values3);
            CHECK(values4.map == values3.map);
            CHECK(values4.data == values3.data);
        }
        SECTION("fully overlapping, all keys are shared, should be b's data") {
            auto values3 = merge(values2, values2);
            CHECK(values3.map == values2.map);
            CHECK(values3.data == values2.data);
        }
        */
        SECTION("merge n values") {}
    }

    WHEN("update and extract") {
        auto values_before_opt = imsym::values::valuesd_t{
            {imsym::key::to(sym_key_0), sym_pose_0},
            {imsym::key::to(sym_key_1), sym_pose_1},
            {imsym::key::key_t{.letter = 'l'}, Pose3d(Rot3d::FromQuaternion({7, 8, 9, 10}), Vector3d{11, 12, 13})},
            {imsym::key::key_t{.letter = 'm'}, 1.2345},
        };

        auto values_after_opt = set(values_before_opt, imsym::key::to(sym_key_1), sym_pose_1_alt);

        values_after_opt = set(values_after_opt, imsym::key::key_t{.letter = 'm'}, 2.34567);
        WHEN("update") {
            auto index_before_opt = create_index(values_before_opt, keys(values_before_opt));
            auto index_after_opt = create_index(values_after_opt, keys(values_after_opt));

            SECTION("using index") {
                auto values_after_update =
                    update(index_before_opt, index_after_opt, values_before_opt, values_after_opt);

                spdlog::error("values_before_opt {}", values_before_opt);
                spdlog::error("values_after_opt {}", values_after_opt);
                spdlog::error("values_after_update {}", values_after_update);
            }
        }

        WHEN("extract") {
            // using a set of keys, make a new values with just that data

            auto idx = create_index(values_before_opt, {imsym::key::to(sym_key_1), imsym::key::key_t{.letter = 'l'}});

            for (const auto i : idx.entries) {
                spdlog::error("idx {}", i);
            }

            auto extracted = extract(values_after_opt, idx);
            spdlog::error("extracted {}", extracted);
        }
    }
}
