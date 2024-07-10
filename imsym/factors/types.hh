/* Copyright (C) Basemap, Inc DBA Automaton  All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Asa Hammond <asa@automaton.is>, 2021
 */

#pragma once

/*
 * common_struct definitions for symforce internal types
 */

#include "common/hash/symforce.hh"
#include "common/struct.hh"

#include <symforce/slam/imu_preintegration/preintegrated_imu_measurements.h>

COMMON_STRUCT_HASH(sym, PreintegratedImuMeasurements<double>::Delta, Dt, DR, Dv, Dp);
COMMON_STRUCT_HASH(sym,
                   PreintegratedImuMeasurements<double>,
                   accel_bias,
                   gyro_bias,
                   delta,
                   DR_D_gyro_bias,
                   Dv_D_accel_bias,
                   Dv_D_gyro_bias,
                   Dp_D_accel_bias,
                   Dp_D_gyro_bias);

