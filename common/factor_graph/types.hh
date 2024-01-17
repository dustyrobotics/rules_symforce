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

#include <lager/extra/cereal/immer_vector.hpp>

#include "common/struct.hh"
namespace factor_graph {

using timestamp_t = size_t;
using factor_index_t = size_t;
using residual_index_t = size_t;

struct coords_t {
    long row;
    long col;
};

struct sparse_matrix_t {
    coords_t size;
    immer::map<coords_t, double> data;
};

struct factor_graph_detailed_internals_t {
    immer::map<imsym::key::key_t, Eigen::MatrixXd> covariances_by_key;

    //    sym::sparse_matrix_structure_t jacobian_sparsity;
    Eigen::VectorXi linear_solver_ordering;
    //    sym::sparse_matrix_structure_t cholesky_factor_sparsity;

    //    from best_iteration
    Eigen::VectorXf residuals;
    // Eigen::SparseMatrix<double> jacobian;
    sparse_matrix_t jacobian;
    // immer::vector<double> residuals;
    immer::vector<factor_index_t> factors_by_residual_idx;
    immer::map<factor_index_t, immer::vector<residual_index_t>> residual_indices_by_factor_id;
};

struct factor_graph_stats_t {
    size_t num_factors = 0;
    int32_t num_values = 0;
    int16_t convergence_iterations = 0;
    float final_error;

    timestamp_t duration;         // how long it took to solve
    timestamp_t last_state_time;  // the time of the last state we solved

    std::optional<factor_graph_detailed_internals_t> internals;
};

struct solve_result_t {
    ::motion::dyn_state_t state = {};
    factor_graph::factor_graph_stats_t solve_stats;
    size_t solve_iterations = 0;
};
}  // namespace factor_graph

COMMON_STRUCT_HASH(factor_graph, coords_t, row, col);

COMMON_STRUCT(factor_graph, sparse_matrix_t, size, data);

COMMON_STRUCT(factor_graph, factor_graph_detailed_internals_t, covariances_by_key, linear_solver_ordering, residuals,
              jacobian, factors_by_residual_idx, residual_indices_by_factor_id);

COMMON_STRUCT(factor_graph, factor_graph_stats_t, num_factors, num_values, convergence_iterations, final_error,
              duration, last_state_time, internals);

COMMON_STRUCT(factor_graph, solve_result_t, state, solve_stats, solve_iterations);
