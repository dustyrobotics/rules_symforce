/* Copyright (C) Basemap, Inc DBA Automaton  All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Asa Hammond <asa@automaton.is>, 2021
 */

#pragma once

#include "imsym/key.hh"
#include "imsym/values.hh"
#include "imsym/values_ops.hh"
//
#include <symforce/opt/key.h>
#include <symforce/opt/values.h>

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

inline auto to(const std::vector<sym::Key>& other) -> immer::vector<imsym::key::key_t> {
    auto out = immer::vector<imsym::key::key_t>{};
    for (const auto k : other) {
        out = std::move(out).push_back(to(k));
    }
    return out;
}

}   // namespace imsym::key

namespace imsym::values {

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

template<typename Scalar, typename T>
inline auto set(values_t<Scalar> values, const imsym::key::key_t& key, const T& value)
    -> values_t<Scalar> {
    // if data is in the map, the entry is overwritten
    // if the data is not, then we append data to the correct location in values.data

    static_assert(std::is_same<Scalar, typename sym::StorageOps<T>::Scalar>::value,
                  "Calling Values.Set on mismatched scalar type.");

    const auto type = sym::StorageOps<T>::TypeEnum();

    if (!values.map.count(key)) {
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
            // TODO make monadic with expected{}
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

// need a storageOps that can read immer::vector

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

    // TODO really just need to be able to set the map and data values directly
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

}   // namespace imsym::values

