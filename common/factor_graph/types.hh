/* Copyright (C) Basemap, Inc DBA Automaton  All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Asa Hammond <asa@automaton.is>, 2022
 */
#pragma once
#include "imsym/key.hh"
#include "motion/types.hh"

#include <immer/map.hpp>

#include <optional>
//
#include "common/cereal/immer_map.hh"
#include "common/cereal/json.hh"
#include "common/cereal/optional_nvp.hh"

#include <cereal/types/optional.hpp>
#include <lager/extra/cereal/immer_vector.hpp>
#include "common/struct.hh"

namespace factor_graph {

using timestamp_t = size_t;
using factor_index_t = int32_t;
using residual_index_t = int32_t;
using dim_t = int32_t;

struct coords_t {
    long row;
    long col;
};

struct sparse_matrix_t {
    coords_t size;
    immer::map<coords_t, double> data;
};

struct dense_matrix_t {
    coords_t size;
    immer::vector<double> data;
};

struct offset_t {
    residual_index_t offset;
    dim_t dim;
};

struct detailed_internals_t {
    immer::map<imsym::key::key_t, dense_matrix_t> covariances_by_key;

    //   sym::sparse_matrix_structure_t jacobian_sparsity;
    immer::vector<size_t> linear_solver_ordering;
    //    sym::sparse_matrix_structure_t cholesky_factor_sparsity;

    //    from best_iteration
    immer::vector<float> residuals;

    sparse_matrix_t jacobian;

    // pair of residual index and dim of the factor residual block
    immer::vector<offset_t> factor_residual_offsets;
};

struct factor_graph_stats_t {
    size_t num_factors = 0;
    int32_t num_values = 0;
    int16_t convergence_iterations = 0;
    float final_error;

    timestamp_t duration;         // how long it took to solve
    timestamp_t last_state_time;  // the time of the last state we solved

    std::optional<detailed_internals_t> internals;
};

struct solve_result_t {
    ::motion::dyn_state_t state = {};
    factor_graph::factor_graph_stats_t solve_stats;
    size_t solve_iterations = 0;
};
}  // namespace factor_graph

COMMON_STRUCT_HASH(factor_graph, coords_t, row, col);

COMMON_STRUCT(factor_graph, sparse_matrix_t, size, data);
COMMON_STRUCT(factor_graph, dense_matrix_t, size, data);

COMMON_STRUCT_HASH(factor_graph, offset_t, offset, dim);

COMMON_STRUCT(factor_graph,            //
              detailed_internals_t,    //
              covariances_by_key,      //
              linear_solver_ordering,  //
              residuals,               //
              jacobian,                //
              factor_residual_offsets  //
);

COMMON_STRUCT(factor_graph, factor_graph_stats_t,
              num_factors,             //
              num_values,              //
              convergence_iterations,  //
              final_error,             //
              duration,                //
              last_state_time,         //
              internals);

COMMON_STRUCT(factor_graph, solve_result_t, state, solve_stats, solve_iterations);
