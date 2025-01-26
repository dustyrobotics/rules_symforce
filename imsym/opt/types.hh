/* Copyright (C) Basemap, Inc DBA Automaton  All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Asa Hammond <asa@automaton.is>, 2022
 */
#pragma once
#include "imsym/opt/key.hh"
#include "imsym/opt/values.hh"
//
//
#include "immer/map.hpp"
#include "immer/vector.hpp"
#include "motion/types.hh"

#include <optional>
#include <variant>
//
/*
#include "common/cereal/immer_map.hh"
#include "common/cereal/json.hh"
#include "common/cereal/optional_nvp.hh"
#include "common/cereal/variant_with_name.hh"
*/
#include "cereal/types/optional.hpp"
#include "common/struct.hh"
#include "lager/extra/cereal/immer_map.hpp"
#include "lager/extra/cereal/immer_vector.hpp"
//
#include "common/enum.hh"
#include "common/hash/immer.hh"
namespace imsym {

using timestamp_t = size_t;
using factor_index_t = int32_t;
using residual_index_t = int32_t;
using dim_t = int32_t;
using std::optional;

struct coords_t {
    long row;
    long col;
};

template<typename Scalar>
struct sparse_matrix {
    coords_t size;
    immer::map<coords_t, Scalar> data;
};

using sparse_matrix_t = sparse_matrix<double>;
using sparse_matrixf_t = sparse_matrix<float>;

template<typename Scalar>
struct dense_matrix {
    // column major dense matrix
    coords_t size;
    immer::vector<Scalar> data;
};

using dense_matrix_t = dense_matrix<double>;
using dense_matrixf_t = dense_matrix<float>;

template<typename Scalar>
struct dense_lt_matrix {
    // column major dense matrix with only the lower triangular part filled in
    coords_t size;
    immer::vector<Scalar> data;
};
using dense_lt_matrix_t = dense_lt_matrix<double>;
using dense_lt_matrixf_t = dense_lt_matrix<float>;

using matrix_t = std::variant<dense_matrix_t, sparse_matrix_t>;
using matrixf_t = std::variant<dense_matrixf_t, sparse_matrixf_t>;

struct offset_t {
    residual_index_t offset;
    dim_t dim;
};

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

// Debug stats for a single iteration of a Levenberg Marquardt optimization
struct optimization_iteration_t {
    // Zero-indexed iteration number (Information before the first iteration is
    // included at index -1)
    int16_t iteration;

    // Value of lambda at this iteration
    double current_lambda;
    // Error after the iteration, using the linearized cost
    double new_error_linear;
    // Error after the iteration, using the full nonlinear cost function
    double new_error;
    // Relative reduction in error between the initial and updated states for
    // this iteration
    double relative_reduction;

    // Was the update accepted?
    bool update_accepted;

    // Angle between previous update and current update
    double update_angle_change;

    // The update, values, residual, and jacobian are only populated when debug_stats is true,
    // otherwise they are size 0

    // The update at this step
    immer::vector<double> update;

    // The Values at this step
    imsym::values::valuesd_t values;

    // The problem residual
    immer::vector<double> residuals;

    // The problem jacobian exactly if dense, or as CSC format sparse data column vector if sparse
    // either sparse or dense jacobian // TODO support the CSC format of sparse data
    matrix_t jacobian;
};

// The structure of a sparse matrix in CSC format, not including the numerical values
// For a description of the format, see
// https://en.wikipedia.org/wiki/Sparse_matrix#Compressed_sparse_column_(CSC_or_CCS)
// In the comments below, assume an M x N matrix with nnz nonzeros
// struct sparse_matrix_structure_t {
//    // The row for each nonzero entry in the matrix
//    eigen_lcm.VectorXi row_indices;   // size nnz
//
//    // The index into row_indices (and the values) of the start of each column
//    eigen_lcm.VectorXi column_pointers;   // size N
//
//    // The shape (M, N) of the sparse matrix
//    int64_t shape[2];
//};

enum optimization_status_t {
    // Uninitialized enum value
    INVALID,
    // The optimization converged successfully
    SUCCESS,
    // We hit the iteration limit before converging
    HIT_ITERATION_LIMIT,
    // The solver failed to converge for some reason (other than hitting the iteration limit)
    FAILED,
};

/*
enum levenberg_marquardt_solver_failure_reason_t : int32_t {
    // Uninitialized enum value
    INVALID = 0,
    // We could not increase lambda high enough to make progress
    LAMBDA_OUT_OF_BOUNDS = 1,
    // The initial error was not finite (either NaN or Inf)
    INITIAL_ERROR_NOT_FINITE = 2,
};
*/

// can be sparse or dense, float or double.
struct sparse_linearization_t {
    // Sparse storage
    immer::vector<double> residual;
    sparse_matrix_t hessian_lower;
    sparse_matrix_t jacobian;
    immer::vector<double> rhs;
};

struct dense_linearization_t {
    // Sparse storage
    immer::vector<double> residual;
    dense_matrix_t hessian_lower;
    dense_matrix_t jacobian;
    immer::vector<double> rhs;
};

using linearization_t = std::variant<dense_linearization_t, sparse_linearization_t>;

// Debug stats for a full optimization run
struct optimization_stats_t {
    immer::vector<optimization_iteration_t> iterations;

    /// Index into iterations of the best iteration (containing the optimal Values)
    int32_t best_index{0};

    /// What was the result of the optimization?
    optimization_status_t status{};

    /// If status == FAILED, why?  This should be cast to the Optimizer::FailureReason enum for the
    /// nonlinear solver you used.
    int32_t failure_reason{};

    /// The linearization at best_index (at optimized_values), filled out if
    /// populate_best_linearization = true
    optional<linearization_t> best_linearization{};

    /// The sparsity pattern of the problem jacobian
    ///
    /// Only filled if Optimizer created with debug_stats = true and include_jacobians = true,
    /// otherwise default constructed.
    ///
    /// If using a dense linearization, only the shape field will be filled.
    // sparse_matrix_t jacobian_sparsity;

    /// The permutation used by the linear solver
    ///
    /// Only filled if using an Optimizer created with debug_stats = true and a linear solver that
    /// exposes Permutation() (such as the default SparseCholeskySolver).  Otherwise, will be
    /// default constructed.
    immer::vector<int> linear_solver_ordering;

    /// The sparsity pattern of the cholesky factor L
    ///
    /// Only filled if using an Optimizer created with debug_stats = true and a linear solver that
    /// exposes L() (such as the default SparseCholeskySolver).  Otherwise, will be default
    /// constructed.
    sparse_matrix_t cholesky_factor_sparsity;
};

using covariance_map_t = immer::map<imsym::key::key_t, dense_matrix_t>;
// using full_covariance_t = dense_lt_matrix_t;
using full_covariance_t = dense_matrix_t;

}   // namespace imsym

COMMON_STRUCT_HASH(imsym, coords_t, row, col);
COMMON_STRUCT_HASH(imsym, offset_t, offset, dim);

COMMON_STRUCT_HASH(imsym, sparse_matrix_t, size, data);
COMMON_STRUCT_HASH(imsym, sparse_matrixf_t, size, data);
COMMON_STRUCT_HASH(imsym, dense_matrix_t, size, data);
COMMON_STRUCT_HASH(imsym, dense_matrixf_t, size, data);
COMMON_STRUCT_HASH(imsym, dense_lt_matrix_t, size, data);
COMMON_STRUCT_HASH(imsym, dense_lt_matrixf_t, size, data);

COMMON_ENUM(imsym, optimization_status_t, INVALID, SUCCESS, HIT_ITERATION_LIMIT, FAILED);

COMMON_STRUCT_HASH(imsym, sparse_linearization_t, residual, hessian_lower, jacobian, rhs);
COMMON_STRUCT_HASH(imsym, dense_linearization_t, residual, hessian_lower, jacobian, rhs);

COMMON_STRUCT(imsym,
              optimization_iteration_t,
              iteration,
              current_lambda,
              new_error_linear,
              new_error,
              relative_reduction,
              update_accepted,
              update_angle_change,
              update,
              values,
              residuals,
              jacobian);   // full jacobian is here

/*
COMMON_ENUM(imsym,
            levenberg_marquardt_solver_failure_reason_t,
            INVALID,
            LAMBDA_OUT_OF_BOUNDS,
            INITIAL_ERROR_NOT_FINITE);
*/

COMMON_STRUCT(imsym,
              optimization_stats_t,
              iterations,           //
              best_index,           //
              status,               //
              failure_reason,       //
              best_linearization,   //
              // jacobian_sparsity,    // jacobian is stored in each iteration directly
              linear_solver_ordering,
              cholesky_factor_sparsity);

