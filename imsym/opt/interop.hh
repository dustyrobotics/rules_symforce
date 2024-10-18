#pragma once
#include "imsym/opt/types.hh"
#include "imsym/opt/values_ext_ops.hh"
#include "imsym/opt/values_ops.hh"

#include <Eigen/Core>
#include <lcmtypes/sym/optimization_iteration_t.hpp>
#include <lcmtypes/sym/optimization_stats_t.hpp>
#include <symforce/opt/key.h>
#include <symforce/opt/optimizer.h>

namespace imsym {

inline auto build_residual_offsets(const auto& factors, const auto& linearizer) {
    auto out = immer::vector<imsym::offset_t>{};
    const auto& offset_and_residual_dim = linearizer.ResidualEntryByFactor();
    for (const auto& [residual_offset, dim] : offset_and_residual_dim) {
        out = std::move(out).push_back({residual_offset, dim});
    }
    return out;
};

template<typename Scalar>
inline auto to_imsym(const Eigen::MatrixX<Scalar>& m) -> dense_matrix<Scalar> {
    dense_matrix<Scalar> out;
    out.size = {.row = m.rows(), .col = m.cols()};
    for (int col = 0; col < m.cols(); col++) {
        for (int row = 0; row < m.rows(); row++) {
            out.data = std::move(out.data).push_back(m(row, col));
        }
    }
    return out;
};

template<typename Scalar>
inline auto to_imsym_matrix(const Eigen::MatrixX<Scalar>& m) -> dense_matrix<Scalar> {
    dense_matrix<Scalar> out;
    out.size = {.row = m.rows(), .col = m.cols()};
    for (int col = 0; col < m.cols(); col++) {
        for (int row = 0; row < m.rows(); row++) {
            out.data = std::move(out.data).push_back(m(row, col));
        }
    }
    return out;
};

template<typename Scalar>
inline auto to_imsym_lt(const Eigen::MatrixX<Scalar>& m) {
    dense_lt_matrix<Scalar> out;
    out.size = {.row = m.rows(), .col = m.cols()};
    for (int col = 0; col < m.cols(); col++) {
        for (int row = col; row < m.rows(); row++) {
            out.data = std::move(out.data).push_back(m(row, col));
        }
    }
    return out;
};

template<typename Scalar>
inline auto to_eigen(const immer::vector<Scalar>& v) {
    Eigen::VectorX<Scalar> out(v.size());
    for (size_t i = 0; i < v.size(); ++i) {
        out[i] = v[i];
    }
    return out;
}

template<typename Scalar>
inline auto to_eigen(const sparse_matrix<Scalar>& m) {
    std::vector<Eigen::Triplet<Scalar>> triplets;

    Eigen::SparseMatrix<Scalar> sparse_mat(m.size.row, m.size.col);

    for (const auto& [coords, v] : m.data) {
        triplets.push_back({coords.row, coords.col, v});
    }
    sparse_mat.setFromTriplets(triplets.begin(), triplets.end());

    return sparse_mat;
};

template<typename Scalar>
inline auto to_eigen(const dense_matrix<Scalar>& m) {
    // column major
    Eigen::MatrixX<Scalar> out(m.size.row, m.size.col);
    auto idx = 0;
    for (int col = 0; col < m.size.col; col++) {
        for (int row = 0; row < m.size.row; row++) {
            out(row, col) = m.data[idx++];
        }
    }
    return out;
};

template<typename Scalar>
inline auto to_eigen(const dense_lt_matrix<Scalar>& m) {
    // TODO a matrixL view?
    // column major
    Eigen::MatrixX<Scalar> out(m.size.row, m.size.col);
    auto idx = 0;
    for (int col = 0; col < m.size.col; col++) {
        for (int row = col; row < m.size.row; row++) {
            out(row, col) = m.data[idx++];
        }
    }
    return out;
};

template<typename Scalar>
inline auto to_imsym(std::unordered_map<sym::Key, Eigen::MatrixX<Scalar>> covariances) {
    auto out = immer::map<imsym::key::key_t, dense_matrix<Scalar>>{};
    for (const auto& [k, v] : covariances) {
        out = std::move(out).set(imsym::key::to(k), to_imsym(v));
    }
    return out;
};

template<typename Scalar>
inline auto to_imsym(const Eigen::SparseMatrix<Scalar>& mat) {
    sparse_matrix<Scalar> out;
    out.size = {mat.rows(), mat.cols()};

    for (int k = 0; k < mat.outerSize(); ++k) {
        for (typename Eigen::SparseMatrix<Scalar>::InnerIterator it(mat, k); it; ++it) {
            // Insert the value into the immer::map with the 2D index (row, col) as the key
            out.data = std::move(out.data).set({it.row(), it.col()}, it.value());
        }
    }

    return out;
};

template<typename Scalar>
inline auto to_imsym(const Eigen::MappedSparseMatrix<Scalar>& sparse_mat) {
    sparse_matrix<Scalar> out;
    out.size = {sparse_mat.rows(), sparse_mat.cols()};

    for (int k = 0; k < sparse_mat.outerSize(); ++k) {
        for (typename Eigen::MappedSparseMatrix<Scalar>::InnerIterator it(sparse_mat, k); it;
             ++it) {
            long row = it.row();
            long col = it.col();
            double value = it.value();

            // spdlog::info("{} {} {}", it.row(), it.col(), it.value());
            //  Insert the value into the immer::map with the 2D index (row, col) as the key
            out.data = std::move(out.data).set({row, col}, value);
        }
    }

    return out;
};

template<typename Scalar>
inline auto to_imsym_matrix(const Eigen::Map<const Eigen::SparseMatrix<Scalar>>& mat)
    -> sparse_matrix<Scalar> {
    return to_imsym(Eigen::SparseMatrix<Scalar>(mat));
};

template<typename Scalar>
inline auto to_imsym_matrix(const Eigen::Map<const Eigen::MatrixX<Scalar>>& mat)
    -> dense_matrix<Scalar> {
    const Eigen::MatrixX<Scalar>& m = mat;
    return to_imsym(m);
};

template<typename Scalar>
inline auto to_imsym(const Eigen::VectorX<Scalar>& v) {
    return immer::vector<Scalar>{v.begin(), v.end()};
};

// could be sparse or dense iteration
inline auto to_imsym(const sym::optimization_iteration_t& iter) -> optimization_iteration_t {
    return {
        .iteration = iter.iteration,
        .current_lambda = iter.current_lambda,
        .new_error_linear = iter.new_error_linear,
        .new_error = iter.new_error,
        .relative_reduction = iter.relative_reduction,
        .update_accepted = iter.update_accepted,
        .update_angle_change = iter.update_angle_change,
        .update = to_imsym(iter.update),
        // .values = imsym::values::clone(iter.values),
        .residuals = to_imsym(iter.residual),
        //.jacobian_values = to_imsym(iter.JacobianValues()),
    };
};

// sparse linearization
template<typename Scalar>
inline auto to_imsym(const sym::SparseLinearization<Scalar>& lin) {
    return sparse_linearization_t{.residual = to_imsym(lin.residual),
                                  .hessian_lower = to_imsym(lin.hessian_lower),
                                  .jacobian = to_imsym(lin.jacobian),
                                  .rhs = to_imsym(lin.rhs)};
}

// dense linearization
template<typename Scalar>
inline auto to_imsym(const sym::DenseLinearization<Scalar>& lin) {
    return dense_linearization_t{};
}

template<typename T>
inline auto to_linearization(const tl::optional<T>& lin) -> optional<linearization_t> {
    if (not lin.has_value()) {
        return {};
    }
    return to_imsym(lin.value());
}

inline auto to_imsym(const sym::optimization_status_t& status) -> optimization_status_t {
    switch (status.value) {
        case sym::optimization_status_t::INVALID:
            return imsym::optimization_status_t::INVALID;
        case sym::optimization_status_t::SUCCESS:
            return imsym::optimization_status_t::SUCCESS;
        case sym::optimization_status_t::HIT_ITERATION_LIMIT:
            return imsym::optimization_status_t::HIT_ITERATION_LIMIT;
        case sym::optimization_status_t::FAILED:
            return imsym::optimization_status_t::FAILED;
    }
};

inline auto build_iterations(const auto& opt_stats, auto jacobians = true)
    -> immer::vector<optimization_iteration_t> {
    auto iterations = immer::vector<optimization_iteration_t>{};

    for (size_t i = 0; i < opt_stats.iterations.size(); i++) {
        spdlog::info("saving iteration {} / {}", i, opt_stats.iterations.size());
        const auto& iter = opt_stats.iterations.at(i);
        auto imsym_iter = to_imsym(iter);
        if (jacobians) {
            // this is only possible if we have the underlying data from the solve
            // this requires the save_jacobians flag to be set in the optimization
            // also the debug_full flag
            // and we have to not have an iteration limit hit. ??
            imsym_iter.jacobian = to_imsym_matrix(opt_stats.JacobianView(iter));
        }
        iterations = std::move(iterations).push_back(imsym_iter);
    }
    return iterations;
};
template<typename MatrixType>
inline auto to_imsym(const sym::OptimizationStats<MatrixType>& opt_stats,
                     bool save_all_iterations = false,
                     bool jacobians = false) -> optimization_stats_t {
    auto iterations = immer::vector<optimization_iteration_t>{};
    if (save_all_iterations) {
        iterations = build_iterations(opt_stats, jacobians);
    }
    return ::imsym::optimization_stats_t{
        .iterations = iterations,
        .best_index = opt_stats.best_index,
        .status = to_imsym(opt_stats.status),
        .failure_reason = opt_stats.failure_reason,
        .best_linearization = to_linearization(opt_stats.best_linearization),
        //.jacobian_sparsity = to_imsym(opt_stats.jacobian_sparsity),
        .linear_solver_ordering = to_imsym(opt_stats.linear_solver_ordering),
        //.cholesky_factor_sparsity = to_imsym(opt_stats.cholesky_factor_sparsity),
    };
};

inline auto to_string(auto g) -> std::string {
    return fmt::format("{}", g);
}
/// just ops on the types

inline auto residuals(const optional<optimization_iteration_t>& iteration)
    -> optional<immer::vector<double>> {
    if (not iteration.has_value()) {
        return {};
    }
    return iteration->residuals;
};

inline auto residuals(const optional<linearization_t>& lin) -> optional<immer::vector<double>> {
    if (not lin.has_value()) {
        return {};
    }
    return mmm::match(move(lin.value()))(
        [&](const sparse_linearization_t& sparse) {
            return sparse.residual;
        },
        [&](const dense_linearization_t& dense) {
            return dense.residual;
        });
};

inline auto jacobian(const optional<optimization_iteration_t>& iteration)
    -> optional<imsym::matrix_t> {
    if (not iteration.has_value()) {
        return {};
    }
    return iteration->jacobian;
};

inline auto jacobian(const optional<linearization_t>& lin) -> optional<imsym::matrix_t> {
    if (not lin.has_value()) {
        return {};
    }
    return mmm::match(move(lin.value()))(
        [&](const sparse_linearization_t& sparse) -> imsym::matrix_t {
            return sparse.jacobian;
        },
        [&](const dense_linearization_t& dense) -> imsym::matrix_t {
            return dense.jacobian;
        });
};
}   // namespace imsym

