/* Copyright (C) Basemap, Inc DBA Automaton  All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Asa Hammond <asa@automaton.is>, 2021
 */

#pragma once
#include "common/immer/utils.hh"
#include "common/struct.hh"
#include "imsym/imsym.hh"

#include <immer/flex_vector.hpp>
#include <immer/map.hpp>
#include <immer/vector.hpp>
#include <lager/context.hpp>
#include <lager/extra/cereal/immer_flex_vector.hpp>
#include <lager/extra/cereal/immer_map.hpp>
#include <lager/extra/cereal/immer_vector.hpp>
#include <lcmtypes/sym/type_t.hpp>
#include <spdlog/spdlog.h>
#include <sym/util/typedefs.h>
#include <symforce/opt/key.h>
#include <symforce/opt/values.h>

#include <variant>
#include <vector>

namespace imsym::key {

/*
 * shim to convert sym->imsym Key
 */

inline auto to(const sym::Key& other) -> key_t {
    return key_t{.letter = other.Letter(), .sub = other.Sub(), .super = other.Super()};
}

// TODO move to utils / interop
inline auto to(const key::key_t& other) -> sym::Key {
    return sym::Key{other.letter, other.sub, other.super};
}

/*
 * convert a vector of sym::Key to a vector of imsym::key_t
 */
inline auto to(const immer::flex_vector<sym::Key>& other) -> immer::flex_vector<key_t> {
    auto out = immer::flex_vector<key_t>{};
    for (const auto k : other) {
        out = std::move(out).push_back(to(k));
    };
    return out;
}

inline auto to(const immer::flex_vector<imsym::key::key_t>& other) -> std::vector<sym::Key> {
    auto out = std::vector<sym::Key>{};
    for (const auto k : other) {
        out.push_back(to(k));
    }
    return out;
}

inline auto to(const immer::vector<imsym::key::key_t>& other) -> std::vector<sym::Key> {
    auto out = std::vector<sym::Key>{};
    for (const auto k : other) {
        out.push_back(to(k));
    }
    return out;
}

}   // namespace imsym::key

/*
 * immutable variant of symforce structures for use in logging and playback
 * they are hashable and convertable to their underlying symforce type
 * TODO: It may be possible to use these as the underlying types in sym::Values in an
 * optimizer of the future
 */

namespace imsym::values {

using std::move;

/*
 * entries for the values_t dict
 */
struct index_entry_t {
    imsym::key::key_t key;
    sym::type_t type;   // type of the underlying storage we should convert this entry to

    // Location within the storage or tangent vector, depending on context
    int32_t offset;

    // Size parameters
    int32_t storage_dim;
    int32_t tangent_dim;
};

struct index_t {
    int32_t storage_dim = 0;
    int32_t tangent_dim = 0;
    immer::flex_vector<index_entry_t> entries;
};

}   // namespace imsym::values

namespace imsym::values {

template<typename Scalar>
using AllowedTypes = std::variant<Scalar,
                                  // sym::type_t::DATABUFFER", databuffers is unsupported ";
                                  // sym::type_t::VECTORX "VectorX is unsupported"
                                  sym::Rot2<Scalar>,
                                  sym::Rot3<Scalar>,
                                  sym::Pose2<Scalar>,
                                  sym::Pose3<Scalar>,
                                  sym::Unit3<Scalar>,
                                  sym::Vector1<Scalar>,
                                  sym::Vector2<Scalar>,
                                  sym::Vector3<Scalar>,
                                  sym::Vector4<Scalar>,
                                  sym::Vector5<Scalar>,
                                  sym::Vector6<Scalar>,
                                  sym::Vector7<Scalar>,
                                  sym::Vector8<Scalar>,
                                  sym::Vector9<Scalar>,
                                  sym::Matrix12<Scalar>,
                                  sym::Matrix13<Scalar>,
                                  sym::Matrix14<Scalar>,
                                  sym::Matrix15<Scalar>,
                                  sym::Matrix16<Scalar>,
                                  sym::Matrix17<Scalar>,
                                  sym::Matrix18<Scalar>,
                                  sym::Matrix19<Scalar>,
                                  sym::Matrix22<Scalar>,
                                  sym::Matrix23<Scalar>,
                                  sym::Matrix24<Scalar>,
                                  sym::Matrix25<Scalar>,
                                  sym::Matrix26<Scalar>,
                                  sym::Matrix27<Scalar>,
                                  sym::Matrix28<Scalar>,
                                  sym::Matrix29<Scalar>,
                                  sym::Matrix32<Scalar>,
                                  sym::Matrix33<Scalar>,
                                  sym::Matrix34<Scalar>,
                                  sym::Matrix35<Scalar>,
                                  sym::Matrix36<Scalar>,
                                  sym::Matrix37<Scalar>,
                                  sym::Matrix38<Scalar>,
                                  sym::Matrix39<Scalar>,
                                  sym::Matrix42<Scalar>,
                                  sym::Matrix43<Scalar>,
                                  sym::Matrix44<Scalar>,
                                  sym::Matrix45<Scalar>,
                                  sym::Matrix46<Scalar>,
                                  sym::Matrix47<Scalar>,
                                  sym::Matrix48<Scalar>,
                                  sym::Matrix49<Scalar>,
                                  sym::Matrix52<Scalar>,
                                  sym::Matrix53<Scalar>,
                                  sym::Matrix54<Scalar>,
                                  sym::Matrix55<Scalar>,
                                  sym::Matrix56<Scalar>,
                                  sym::Matrix57<Scalar>,
                                  sym::Matrix58<Scalar>,
                                  sym::Matrix59<Scalar>,
                                  sym::Matrix62<Scalar>,
                                  sym::Matrix63<Scalar>,
                                  sym::Matrix64<Scalar>,
                                  sym::Matrix65<Scalar>,
                                  sym::Matrix66<Scalar>,
                                  sym::Matrix67<Scalar>,
                                  sym::Matrix68<Scalar>,
                                  sym::Matrix69<Scalar>,
                                  sym::Matrix72<Scalar>,
                                  sym::Matrix73<Scalar>,
                                  sym::Matrix74<Scalar>,
                                  sym::Matrix75<Scalar>,
                                  sym::Matrix76<Scalar>,
                                  sym::Matrix77<Scalar>,
                                  sym::Matrix78<Scalar>,
                                  sym::Matrix79<Scalar>,
                                  sym::Matrix82<Scalar>,
                                  sym::Matrix83<Scalar>,
                                  sym::Matrix84<Scalar>,
                                  sym::Matrix85<Scalar>,
                                  sym::Matrix86<Scalar>,
                                  sym::Matrix87<Scalar>,
                                  sym::Matrix88<Scalar>,
                                  sym::Matrix89<Scalar>,
                                  sym::Matrix92<Scalar>,
                                  sym::Matrix93<Scalar>,
                                  sym::Matrix94<Scalar>,
                                  sym::Matrix95<Scalar>,
                                  sym::Matrix96<Scalar>,
                                  sym::Matrix97<Scalar>,
                                  sym::Matrix98<Scalar>,
                                  sym::Matrix99<Scalar>,
                                  sym::ATANCameraCal<Scalar>,
                                  sym::DoubleSphereCameraCal<Scalar>,
                                  sym::EquirectangularCameraCal<Scalar>,
                                  sym::LinearCameraCal<Scalar>,
                                  sym::PolynomialCameraCal<Scalar>,
                                  sym::SphericalCameraCal<Scalar>>;

/*
 * a la sym::Valuesd
 * a map of keys to values and the scalar data underlying it
 * values are stored in a flex_vector
 */
template<typename Scalar>
struct values_t {
    using map_t = immer::map<imsym::key::key_t, index_entry_t>;
    using data_t = immer::flex_vector<Scalar>;
    map_t map;
    data_t data;

    values_t(std::initializer_list<std::tuple<imsym::key::key_t, AllowedTypes<Scalar>>> init_list);

    values_t(){};
};

using valuesd_t = values_t<double>;
using valuesf_t = values_t<float>;

template<typename Scalar, typename T>
inline auto set(values_t<Scalar> values, const imsym::key::key_t& key, const T& value)
    -> values_t<Scalar> {
    // if data is in the map, the entry is overwritten
    // if the data is not, then we append data to the correct location in values.data

    static_assert(std::is_same<Scalar, typename sym::StorageOps<T>::Scalar>::value,
                  "Calling Values.Set on mismatched scalar type.");

    const auto type = sym::StorageOps<T>::TypeEnum();
    // bool is_new = false;

    if (!values.map.count(key)) {
        // is_new = true;
        auto entry = values::index_entry_t{};
        entry.key = key;
        entry.type = type;
        entry.offset = static_cast<int32_t>(values.data.size());
        entry.storage_dim = sym::StorageOps<T>::StorageDim();
        entry.tangent_dim = sym::LieGroupOps<T>::TangentDim();

        // Extend end of data
        // data_.insert(data_.end(), entry.storage_dim, 0);

        const auto new_vec = immer::vector<Scalar>(entry.storage_dim, 0.0);

        values.data = values.data + new_vec;

        values.map = move(values.map).set(key, entry);

    } else {
        if (values.map.at(key).type != type) {
            // TODO(hayk): Return an error enum instead of an exception?
            throw std::runtime_error("Calling Set on the wrong value type.");
        }
    }

    const auto& entry = values.map.at(key);
    // overwrite the given location in data

    // Save the value
    std::vector<Scalar> temp_array(entry.storage_dim);
    sym::StorageOps<T>::ToStorage(value, temp_array.data());

    for (size_t i = 0; i < temp_array.size(); i++) {
        values.data = move(values.data).set(entry.offset + i, temp_array.at(i));
    }

    // return is_new;
    return values;
};

template<typename Scalar, typename T>
inline auto set(sym::Values<Scalar>& values, key::key_t key, const T& value) {
    values.Set(to(key), value);
    return values;
};

template<typename Scalar>
inline auto remove(values_t<Scalar> values, imsym::key::key_t key) -> values_t<Scalar> {
    values.map = move(values.map).erase(key);
    // leave data alone, it can be cleaned up manually later with the Cleanup() method
    return values;
};

/**
 * Create an index from the given ordered subset of keys. This object can then be used
 * for repeated efficient operations on that subset of keys.
 *
 * If you want an index of all the keys, call `values.CreateIndex(values.Keys())`.
 *
 * An index will be INVALIDATED if the following happens:
 *  1) Remove() is called with a contained key, or RemoveAll() is called
 *  2) Cleanup() is called to re-pack the data array
 *
 * NOTE(hayk): We could also add a simple UpdateIndex(&index) method, since the offset is the
 * only thing that needs to get updated after repacking.
 */

template<typename Scalar>
inline auto create_index(const values_t<Scalar>& values,
                         const immer::vector<imsym::key::key_t>& keys) -> index_t {
    index_t index{.storage_dim = 0, .tangent_dim = 0};

    for (const auto& key : keys) {
        const auto& val = values.map.find(key);

        if (val == nullptr) {
            throw std::runtime_error(
                fmt::format("Tried to create index for key {} not in values", key));
        }

        const auto entry = *val;
        index.entries = move(index.entries).push_back(entry);
        index.storage_dim += entry.storage_dim;
        index.tangent_dim += entry.tangent_dim;
    }

    return index;
};

// ----------------------------------------------------------------------------
// Public Methods
// ----------------------------------------------------------------------------
template<typename Scalar>
auto has(const values_t<Scalar>& values, const key::key_t& key) -> bool {
    return values.map.count(key);
}

template<typename Scalar>
auto find_letter_sub(const values_t<Scalar>& values, const key::key_t& key)
    -> std::optional<key::key_t> {
    for (const auto& [k, v] : values.map) {
        if (k.letter == key.letter and k.sub == key.sub) {
            return k;
        }
    }
    return {};
}

// need a storageOps that can read immer::vector

template<typename Scalar, typename T>
auto at(const values_t<Scalar>& values, const index_entry_t& entry) -> T {
    // Check the type
    const sym::type_t type = sym::StorageOps<T>::TypeEnum();

    if (entry.type != type) {
        throw std::runtime_error(
            fmt::format("Mismatched types; index entry is type {}, T is {}", entry.type, type));
    }

    auto slice = values.data.drop(entry.offset).take(entry.storage_dim);
    if (slice.size() < entry.tangent_dim) {
        throw std::runtime_error(
            fmt::format("not enough data {} {} to load data", slice.size(), entry.storage_dim));
    }

    // slice off the bit of entry
    const auto data = common::immer::to_std(slice);
    // Construct the object
    return sym::StorageOps<T>::FromStorage(data.data());
}

template<typename Scalar, typename T>
auto at(const values_t<Scalar>& values, const key::key_t& key) -> T {
    auto entry = values.map.at(key);
    // TODO handle failure
    return at<T>(values, entry);
}

template<typename T>
auto at(const valuesd_t& values, const index_entry_t& entry) -> T {
    return at<double, T>(values, entry);
}

template<typename T>
auto at(const valuesf_t& values, const index_entry_t& entry) -> T {
    return at<float, T>(values, entry);
}

template<typename T>
auto at(const valuesd_t& values, const key::key_t& key) -> T {
    return at<double, T>(values, key);
}

template<typename T>
auto at(const valuesf_t& values, const key::key_t& key) -> T {
    return at<float, T>(values, key);
}

template<typename Scalar>
inline auto keys(const values_t<Scalar>& values) {
    return keys<Scalar>(values.map);
};

template<typename Scalar>
inline auto keys(typename values_t<Scalar>::map_t map, const bool sort_by_offset = true) {
    // Sort the keys by offset so iterating through is saner and more memory friendly
    if (sort_by_offset) {
        std::vector<imsym::key::key_t> keys;
        keys.reserve(map.size());

        for (const auto& [k, v] : map) {
            keys.push_back(k);
        }
        std::sort(keys.begin(), keys.end(), [&](const auto& a, const auto& b) {
            return map.at(a).offset < map.at(b).offset;
        });
        return common::immer::vector_from_std(keys);
    }

    immer::vector<imsym::key::key_t> keys;
    for (const auto& [k, v] : map) {
        keys = move(keys).push_back(k);
    }
    return keys;
};

/**
 * Repack the data array to get rid of empty space from removed keys. If regularly removing
 * keys, it's up to the user to call this appropriately to avoid storage growth. Returns the
 * number of Scalar elements cleaned up from the data array.
 *
 * It will INVALIDATE all indices, offset increments, and pointers.
 * Re-create an index with CreateIndex().
 */
template<typename Scalar>
inline auto cleanup(values_t<Scalar> values) -> std::pair<decltype(values), size_t> {
    // Copy the original data
    const auto data_copy = values.data;

    // Build an index of all keys
    const auto full_index = create_index(values, keys<Scalar>(values));

    // initialize all the data
    values.data = immer::vector<Scalar>(full_index.storage_dim, Scalar{});

    SYM_ASSERT(data_copy.size() >= values.data.size());

    // Copy into new data and update the offset in the map
    int32_t new_offset = 0;
    for (index_entry_t entry : full_index.entries) {
        // copy data over
        for (int i = entry.offset, j = new_offset; i < (entry.offset + entry.storage_dim);
             i++, j++) {
            /*
        spdlog::debug(
            "setting entry {} from original entry {} from entry.offset {} storage_dim {}",
            j,
            i,
            entry.offset,
            entry.storage_dim);
        */
            values.data = move(values.data).set(j, data_copy.at(i));
        }

        entry.offset = new_offset;
        values.map = move(values.map).set(entry.key, entry);

        new_offset += entry.storage_dim;
    }
    return std::pair<decltype(values), size_t>{values, data_copy.size() - values.data.size()};
};

/*
 * shim to convert sym->imsym index_entry_t
 */
inline auto to(const sym::index_entry_t& other) -> values::index_entry_t {
    return values::index_entry_t{
        .key = key::to(other.key),
        .type = other.type,
        .offset = other.offset,
        .storage_dim = other.storage_dim,
        .tangent_dim = other.tangent_dim,
    };
}

/*
 * perfect copy of a sym::Values data structure as an immutable imsym version
 */
template<typename Scalar>
inline auto clone(const sym::Values<Scalar>& other) -> values_t<Scalar> {
    auto values = imsym::values::values_t<Scalar>{};

    for (const auto& [k, v] : other.Items()) {
        values.map = move(values.map).set(key::to(k), to(v));
    }

    for (const auto& v : other.Data()) {
        values.data = move(values.data).push_back(v);
    }

    return values;
}

/*
 * perfect copy of an imsym data structure as a sym::Values
 */
template<typename Scalar>
inline auto clone(const values_t<Scalar>& other) -> sym::Values<Scalar> {
    auto values = sym::Values<Scalar>{};

    // TODO really just need to be able to set they map and data values directly
    // here we set the values one at a time
    for (const auto& entry : create_index(other, keys<Scalar>(other)).entries) {
        const auto& key = entry.key;
        // need the type of k
        auto handle_switch = [&]<typename T>() {
            const auto v = at<T>(other, entry);
            values.Set(key::to(key), v);
        };
        switch (entry.type.value) {
            case sym::type_t::INVALID:
                throw "invalid type specified in values";
            case sym::type_t::SCALAR:
                handle_switch.template operator()<Scalar>();
                break;
            case sym::type_t::ROT2:
                handle_switch.template operator()<sym::Rot2<Scalar>>();
                break;
            case sym::type_t::ROT3:
                handle_switch.template operator()<sym::Rot3<Scalar>>();
                break;
            case sym::type_t::POSE2:
                handle_switch.template operator()<sym::Pose2<Scalar>>();
                break;
            case sym::type_t::POSE3:
                handle_switch.template operator()<sym::Pose3<Scalar>>();
                break;
            case sym::type_t::UNIT3:
                handle_switch.template operator()<sym::Unit3<Scalar>>();
                break;
            case sym::type_t::DATABUFFER:
                throw "databuffers is unsupported in values cloning";
                break;
            case sym::type_t::VECTORX:
                throw "VectorX is unsupported in values cloning";
            case sym::type_t::VECTOR1:
                handle_switch.template operator()<sym::Vector1<Scalar>>();
                break;
            case sym::type_t::VECTOR2:
                handle_switch.template operator()<sym::Vector2<Scalar>>();
                break;
            case sym::type_t::VECTOR3:
                handle_switch.template operator()<sym::Vector3<Scalar>>();
                break;
            case sym::type_t::VECTOR4:
                handle_switch.template operator()<sym::Vector4<Scalar>>();
                break;
            case sym::type_t::VECTOR5:
                handle_switch.template operator()<sym::Vector5<Scalar>>();
                break;
            case sym::type_t::VECTOR6:
                handle_switch.template operator()<sym::Vector6<Scalar>>();
                break;
            case sym::type_t::VECTOR7:
                handle_switch.template operator()<sym::Vector7<Scalar>>();
                break;
            case sym::type_t::VECTOR8:
                handle_switch.template operator()<sym::Vector8<Scalar>>();
                break;
            case sym::type_t::VECTOR9:
                handle_switch.template operator()<sym::Vector9<Scalar>>();
                break;
            case sym::type_t::MATRIX12:
                handle_switch.template operator()<sym::Matrix12<Scalar>>();
                break;
            case sym::type_t::MATRIX13:
                handle_switch.template operator()<sym::Matrix13<Scalar>>();
                break;
            case sym::type_t::MATRIX14:
                handle_switch.template operator()<sym::Matrix14<Scalar>>();
                break;
            case sym::type_t::MATRIX15:
                handle_switch.template operator()<sym::Matrix15<Scalar>>();
                break;
            case sym::type_t::MATRIX16:
                handle_switch.template operator()<sym::Matrix16<Scalar>>();
                break;
            case sym::type_t::MATRIX17:
                handle_switch.template operator()<sym::Matrix17<Scalar>>();
                break;
            case sym::type_t::MATRIX18:
                handle_switch.template operator()<sym::Matrix18<Scalar>>();
                break;
            case sym::type_t::MATRIX19:
                handle_switch.template operator()<sym::Matrix19<Scalar>>();
                break;
            case sym::type_t::MATRIX22:
                handle_switch.template operator()<sym::Matrix22<Scalar>>();
                break;
            case sym::type_t::MATRIX23:
                handle_switch.template operator()<sym::Matrix23<Scalar>>();
                break;
            case sym::type_t::MATRIX24:
                handle_switch.template operator()<sym::Matrix24<Scalar>>();
                break;
            case sym::type_t::MATRIX25:
                handle_switch.template operator()<sym::Matrix25<Scalar>>();
                break;
            case sym::type_t::MATRIX26:
                handle_switch.template operator()<sym::Matrix26<Scalar>>();
                break;
            case sym::type_t::MATRIX27:
                handle_switch.template operator()<sym::Matrix27<Scalar>>();
                break;
            case sym::type_t::MATRIX28:
                handle_switch.template operator()<sym::Matrix28<Scalar>>();
                break;
            case sym::type_t::MATRIX29:
                handle_switch.template operator()<sym::Matrix29<Scalar>>();
                break;
            case sym::type_t::MATRIX32:
                handle_switch.template operator()<sym::Matrix32<Scalar>>();
                break;
            case sym::type_t::MATRIX33:
                handle_switch.template operator()<sym::Matrix33<Scalar>>();
                break;
            case sym::type_t::MATRIX34:
                handle_switch.template operator()<sym::Matrix34<Scalar>>();
                break;
            case sym::type_t::MATRIX35:
                handle_switch.template operator()<sym::Matrix35<Scalar>>();
                break;
            case sym::type_t::MATRIX36:
                handle_switch.template operator()<sym::Matrix36<Scalar>>();
                break;
            case sym::type_t::MATRIX37:
                handle_switch.template operator()<sym::Matrix37<Scalar>>();
                break;
            case sym::type_t::MATRIX38:
                handle_switch.template operator()<sym::Matrix38<Scalar>>();
                break;
            case sym::type_t::MATRIX39:
                handle_switch.template operator()<sym::Matrix39<Scalar>>();
                break;
            case sym::type_t::MATRIX42:
                handle_switch.template operator()<sym::Matrix42<Scalar>>();
                break;
            case sym::type_t::MATRIX43:
                handle_switch.template operator()<sym::Matrix43<Scalar>>();
                break;
            case sym::type_t::MATRIX44:
                handle_switch.template operator()<sym::Matrix44<Scalar>>();
                break;
            case sym::type_t::MATRIX45:
                handle_switch.template operator()<sym::Matrix45<Scalar>>();
                break;
            case sym::type_t::MATRIX46:
                handle_switch.template operator()<sym::Matrix46<Scalar>>();
                break;
            case sym::type_t::MATRIX47:
                handle_switch.template operator()<sym::Matrix47<Scalar>>();
                break;
            case sym::type_t::MATRIX48:
                handle_switch.template operator()<sym::Matrix48<Scalar>>();
                break;
            case sym::type_t::MATRIX49:
                handle_switch.template operator()<sym::Matrix49<Scalar>>();
                break;
            case sym::type_t::MATRIX52:
                handle_switch.template operator()<sym::Matrix52<Scalar>>();
                break;
            case sym::type_t::MATRIX53:
                handle_switch.template operator()<sym::Matrix53<Scalar>>();
                break;
            case sym::type_t::MATRIX54:
                handle_switch.template operator()<sym::Matrix54<Scalar>>();
                break;
            case sym::type_t::MATRIX55:
                handle_switch.template operator()<sym::Matrix55<Scalar>>();
                break;
            case sym::type_t::MATRIX56:
                handle_switch.template operator()<sym::Matrix56<Scalar>>();
                break;
            case sym::type_t::MATRIX57:
                handle_switch.template operator()<sym::Matrix57<Scalar>>();
                break;
            case sym::type_t::MATRIX58:
                handle_switch.template operator()<sym::Matrix58<Scalar>>();
                break;
            case sym::type_t::MATRIX59:
                handle_switch.template operator()<sym::Matrix59<Scalar>>();
                break;
            case sym::type_t::MATRIX62:
                handle_switch.template operator()<sym::Matrix62<Scalar>>();
                break;
            case sym::type_t::MATRIX63:
                handle_switch.template operator()<sym::Matrix63<Scalar>>();
                break;
            case sym::type_t::MATRIX64:
                handle_switch.template operator()<sym::Matrix64<Scalar>>();
                break;
            case sym::type_t::MATRIX65:
                handle_switch.template operator()<sym::Matrix65<Scalar>>();
                break;
            case sym::type_t::MATRIX66:
                handle_switch.template operator()<sym::Matrix66<Scalar>>();
                break;
            case sym::type_t::MATRIX67:
                handle_switch.template operator()<sym::Matrix67<Scalar>>();
                break;
            case sym::type_t::MATRIX68:
                handle_switch.template operator()<sym::Matrix68<Scalar>>();
                break;
            case sym::type_t::MATRIX69:
                handle_switch.template operator()<sym::Matrix69<Scalar>>();
                break;
            case sym::type_t::MATRIX72:
                handle_switch.template operator()<sym::Matrix72<Scalar>>();
                break;
            case sym::type_t::MATRIX73:
                handle_switch.template operator()<sym::Matrix73<Scalar>>();
                break;
            case sym::type_t::MATRIX74:
                handle_switch.template operator()<sym::Matrix74<Scalar>>();
                break;
            case sym::type_t::MATRIX75:
                handle_switch.template operator()<sym::Matrix75<Scalar>>();
                break;
            case sym::type_t::MATRIX76:
                handle_switch.template operator()<sym::Matrix76<Scalar>>();
                break;
            case sym::type_t::MATRIX77:
                handle_switch.template operator()<sym::Matrix77<Scalar>>();
                break;
            case sym::type_t::MATRIX78:
                handle_switch.template operator()<sym::Matrix78<Scalar>>();
                break;
            case sym::type_t::MATRIX79:
                handle_switch.template operator()<sym::Matrix79<Scalar>>();
                break;
            case sym::type_t::MATRIX82:
                handle_switch.template operator()<sym::Matrix82<Scalar>>();
                break;
            case sym::type_t::MATRIX83:
                handle_switch.template operator()<sym::Matrix83<Scalar>>();
                break;
            case sym::type_t::MATRIX84:
                handle_switch.template operator()<sym::Matrix84<Scalar>>();
                break;
            case sym::type_t::MATRIX85:
                handle_switch.template operator()<sym::Matrix85<Scalar>>();
                break;
            case sym::type_t::MATRIX86:
                handle_switch.template operator()<sym::Matrix86<Scalar>>();
                break;
            case sym::type_t::MATRIX87:
                handle_switch.template operator()<sym::Matrix87<Scalar>>();
                break;
            case sym::type_t::MATRIX88:
                handle_switch.template operator()<sym::Matrix88<Scalar>>();
                break;
            case sym::type_t::MATRIX89:
                handle_switch.template operator()<sym::Matrix89<Scalar>>();
                break;
            case sym::type_t::MATRIX92:
                handle_switch.template operator()<sym::Matrix92<Scalar>>();
                break;
            case sym::type_t::MATRIX93:
                handle_switch.template operator()<sym::Matrix93<Scalar>>();
                break;
            case sym::type_t::MATRIX94:
                handle_switch.template operator()<sym::Matrix94<Scalar>>();
                break;
            case sym::type_t::MATRIX95:
                handle_switch.template operator()<sym::Matrix95<Scalar>>();
                break;
            case sym::type_t::MATRIX96:
                handle_switch.template operator()<sym::Matrix96<Scalar>>();
                break;
            case sym::type_t::MATRIX97:
                handle_switch.template operator()<sym::Matrix97<Scalar>>();
                break;
            case sym::type_t::MATRIX98:
                handle_switch.template operator()<sym::Matrix98<Scalar>>();
                break;
            case sym::type_t::MATRIX99:
                handle_switch.template operator()<sym::Matrix99<Scalar>>();
                break;
            case sym::type_t::ATAN_CAMERA_CAL:
                handle_switch.template operator()<sym::ATANCameraCal<Scalar>>();
                break;
            case sym::type_t::DOUBLE_SPHERE_CAMERA_CAL:
                handle_switch.template operator()<sym::DoubleSphereCameraCal<Scalar>>();
                break;
            case sym::type_t::EQUIRECTANGULAR_CAMERA_CAL:
                handle_switch.template operator()<sym::EquirectangularCameraCal<Scalar>>();
                break;
            case sym::type_t::LINEAR_CAMERA_CAL:
                handle_switch.template operator()<sym::LinearCameraCal<Scalar>>();
                break;
            case sym::type_t::POLYNOMIAL_CAMERA_CAL:
                handle_switch.template operator()<sym::PolynomialCameraCal<Scalar>>();
                break;
            case sym::type_t::SPHERICAL_CAMERA_CAL:
                handle_switch.template operator()<sym::SphericalCameraCal<Scalar>>();
                break;
        }
    }
    return values;
}

template<typename Scalar>
inline auto merge(const values_t<Scalar>& a, const values_t<Scalar>& b) -> values_t<Scalar> {
    auto out = a;
    for (const auto& [k, v] : b.map) {
        auto v_out = v;
        // UPDATE THE POSITION OF THE ENTRY by length of a.data
        // we are appending all the data from b to a
        v_out.offset += a.data.size();
        out.map = move(out.map).set(k, v_out);
    }

    for (const auto& v : b.data) {
        out.data = move(out.data).push_back(v);
    }
    return out;
}

template<typename Scalar>
inline auto merge(const values_t<Scalar>& a, const values_t<Scalar>& b, const values_t<Scalar>& c)
    -> values_t<Scalar> {
    return merge(merge(a, b), c);
}

/**
 * Efficiently update the keys from a different structured values, given by
 * `index_a` and `index_b`. This purely copies slices of the data arrays.
 *
 * `index_a` MUST be valid for this object; `index_b` MUST be valid for other object.
 */
template<typename Scalar>
inline values_t<Scalar> update(const index_t& index_a,
                               const index_t& index_b,
                               const values_t<Scalar>& values_a,
                               const values_t<Scalar>& values_b) {
    auto values_out = values_a;
    assert(index_a.entries.size() == index_b.entries.size());
    for (int i = 0; i < static_cast<int>(index_a.entries.size()); ++i) {
        const index_entry_t& entry_a = index_a.entries[i];
        const index_entry_t& entry_b = index_b.entries[i];
        assert(entry_a.storage_dim == entry_b.storage_dim);
        assert(entry_a.key == entry_b.key);

        auto pre_a = values_a.data.take(entry_a.offset);
        auto post_a = values_a.data.drop(entry_a.offset + entry_a.storage_dim);

        auto slice_b = values_b.data.drop(entry_b.offset).take(entry_b.storage_dim);

        values_out.data = pre_a + slice_b + post_a;
    }
    return values_out;
}

/*
 * update a single key from other
 * this avoids having to know about type info
 * key doesn't exist on the a side
 */
template<typename Scalar>
inline values_t<Scalar> copy_data_for_new_key(const values_t<Scalar>& values_a,
                                              const values_t<Scalar>& values_b,
                                              const imsym::key::key_t& key) {
    auto values_out = values_a;
    auto entry_b = values_b.map.at(key);

    // the new data to insert
    const auto slice_b = values_b.data.drop(entry_b.offset).take(entry_b.storage_dim);
    entry_b.offset = values_out.data.size();
    values_out.data = move(values_out.data) + slice_b;
    values_out.map = move(values_out.map).set(key, entry_b);

    return values_out;
}

// data has to be the same size for both sides
// we just insert the data directly over the old data
// key has to exist on both sides
template<typename Scalar>
inline values_t<Scalar> copy_data_for_existing_key(const values_t<Scalar>& values_a,
                                                   const values_t<Scalar>& values_b,
                                                   const imsym::key::key_t& key) {
    auto values_out = values_a;

    const auto& entry_a = values_a.map.at(key);
    const auto& entry_b = values_b.map.at(key);
    assert(entry_a.storage_dim == entry_b.storage_dim);
    assert(entry_a.key == entry_b.key);

    auto pre_a = values_a.data.take(entry_a.offset);
    auto post_a = values_a.data.drop(entry_a.offset + entry_a.storage_dim);
    auto slice_b = values_b.data.drop(entry_b.offset).take(entry_b.storage_dim);

    values_out.data = pre_a + slice_b + post_a;

    return values_out;
}
/*
 * update a single key from other
 * this avoids having to know about type info
 * if key exists on both sides, straightforward
 * if key exists in other
 * - copy the data from other
 */
template<typename Scalar>
inline values_t<Scalar>
    update(const values_t<Scalar>& values_a, const values_t<Scalar>& values_b, const key_t& key) {
    auto values_out = values_a;

    const auto& entry_a = values_a.map.get(key);
    const auto& entry_b = values_b.map.get(key);

    assert(entry_a.storage_dim == entry_b.storage_dim);
    assert(entry_a.key == entry_b.key);

    const auto pre_a = values_a.data.take(entry_a.offset);
    const auto post_a = values_a.data.drop(entry_a.offset + entry_a.storage_dim);

    // the new data to insert
    const auto slice_b = values_b.data.drop(entry_b.offset).take(entry_b.storage_dim);

    values_out.data = pre_a + slice_b + post_a;

    return values_out;
}
/**
 * extract values from the given keys into a new values
 */
template<typename Scalar>
inline values_t<Scalar> extract(const values_t<Scalar>& other, const index_t& index) {
    auto values = values_t<Scalar>{};

    for (const auto& other_entry : index.entries) {
        auto entry = other_entry;
        entry.offset = static_cast<int32_t>(values.data.size());

        const auto other_slice = other.data.drop(other_entry.offset).take(other_entry.storage_dim);

        values.data = values.data + other_slice;
        values.map = move(values.map).set(entry.key, entry);
    }
    return values;
}

template<typename Scalar>
values_t<Scalar>::values_t(
    std::initializer_list<std::tuple<imsym::key::key_t, AllowedTypes<Scalar>>> init_list) {
    values_t<Scalar> me = *this;
    for (const auto& [k, v] : init_list) {
        me = lager::match(v)([me = move(me), k = k](auto a) {
            return set(move(me), k, a);
        });
    }
    map = move(me.map);
    data = move(me.data);
};

}   // namespace imsym::values

template<>
struct fmt::formatter<imsym::values::index_entry_t> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const imsym::values::index_entry_t& i, FormatContext& ctx) {
        return fmt::format_to(
            ctx.out(),
            "index_entry_t: key: {} offset: {} storage_dim: {} tangent_dim: {} type: {}",
            i.key,
            i.offset,
            i.storage_dim,
            i.tangent_dim,
            i.type);
    }
};

template<>
struct fmt::formatter<imsym::values::index_t> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const imsym::values::index_t& i, FormatContext& ctx) {
        return fmt::format_to(ctx.out(),
                              "index_t: {} storage_dim: {} tangent_dim: {} entries: {}",
                              i.storage_dim,
                              i.tangent_dim,
                              i.entries);
    }
};

template<>
struct fmt::formatter<imsym::values::valuesd_t> {
    static constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const imsym::values::valuesd_t& values, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "values\n");

        const auto full_index =
            create_index(values, keys<double>(values.map, true));   // sort by offset
                                                                    //
        for (const auto& entry : full_index.entries) {
            fmt::format_to(ctx.out(),
                           "\t{} {}\n",
                           entry,
                           values.data.drop(entry.offset).take(entry.storage_dim));
        }

        return fmt::format_to(ctx.out(), "done");
    }
};

// TODO move this to a sym archive file?
namespace cereal {
template<class Archive>
void save(Archive& archive, const sym::type_t& m) {
    archive(static_cast<int32_t>(m.value));
}

template<class Archive>
void load(Archive& archive, sym::type_t& m) {
    int32_t value;
    archive(value);
    m.value = static_cast<sym::type_t::option_t>(value);
}
}   // namespace cereal

COMMON_STRUCT(imsym::values, index_entry_t, key, type, offset, storage_dim, tangent_dim);

COMMON_STRUCT(imsym::values, index_t, storage_dim, tangent_dim, entries);

COMMON_STRUCT(imsym::values, valuesd_t, map, data);
COMMON_STRUCT(imsym::values, valuesf_t, map, data);

namespace imsym::values {

// perhaps move this to a detail namespace

/*
 * check equality of map contents
 * this is different than object equality offered by operator == on the map which checks for
 * identity() through map tree operations
 */
inline auto contents_equal(const immer::map<imsym::key::key_t, index_entry_t>& a,
                           const immer::map<imsym::key::key_t, index_entry_t>& b) -> bool {
    for (const auto& [key, entry] : a) {
        if (b.count(key) == 0) {
            return false;
        }
        // entry exists in b
        const index_entry_t& v_b = b.at(key);
        if (entry != v_b) {
            return false;
        }
    }
    // check back in the other direction
    for (const auto& [key, entry] : b) {
        if (a.count(key) == 0) {
            return false;
        }
        // entry exists in a
        if (entry != a.at(key)) {
            return false;
        }
    }
    return true;
}

/* check equality of map and data
 * this is different than object equality offered by operator == on the map which checks for
 * identity()
 */
template<typename Scalar>
inline auto contents_equal(const values_t<Scalar>& a, const values_t<Scalar>& b) -> bool {
    return contents_equal(a.map, b.map) and (a.data == b.data);
}
}   // namespace imsym::values
