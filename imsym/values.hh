/* Copyright (C) Basemap, Inc DBA Automaton  All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Asa Hammond <asa@automaton.is>, 2021
 */

#pragma once
#include "common/immer/utils.hh"
#include "common/struct.hh"
#include "imsym/key.hh"

//
#include <immer/flex_vector.hpp>
#include <immer/map.hpp>
//
#include <lcmtypes/sym/type_t.hpp>
#include <sym/atan_camera_cal.h>
#include <sym/camera.h>
#include <sym/double_sphere_camera_cal.h>
#include <sym/equirectangular_camera_cal.h>
#include <sym/linear_camera_cal.h>
#include <sym/polynomial_camera_cal.h>
#include <sym/pose2.h>
#include <sym/pose3.h>
#include <sym/posed_camera.h>
#include <sym/rot2.h>
#include <sym/rot3.h>
#include <sym/spherical_camera_cal.h>
#include <sym/unit3.h>
#include <sym/util/typedefs.h>
////
#include <vector>
/*
 * immutable variant of symforce structures for use in logging and playback
 * they are hashable and convertable to their underlying symforce type
 * TODO: It may be possible to use these as the underlying types in sym::Values in an
 * optimizer of the future
 */

namespace imsym::values {

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

    constexpr size_t num_entries() const {
        return map.size();
    };
};

using valuesd_t = values_t<double>;
using valuesf_t = values_t<float>;

}   // namespace imsym::values

COMMON_STRUCT(imsym::values, index_entry_t, key, type, offset, storage_dim, tangent_dim);

COMMON_STRUCT(imsym::values, index_t, storage_dim, tangent_dim, entries);

COMMON_STRUCT(imsym::values, valuesd_t, map, data);
COMMON_STRUCT(imsym::values, valuesf_t, map, data);

