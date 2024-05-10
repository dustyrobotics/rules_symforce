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

inline auto to_imsym(const Eigen::MatrixXd& m) -> dense_matrix_t {
    dense_matrix_t out;
    out.size = {.row = m.rows(), .col = m.cols()};
    for (int col = 0; col < m.cols(); col++) {
        for (int row = 0; row < m.rows(); row++) {
            out.data = std::move(out.data).push_back(m(row, col));
        }
    }
    return out;
};

inline auto to_eigen(const dense_matrix_t& m) -> Eigen::MatrixXd {
    Eigen::MatrixXd out(m.size.row, m.size.col);
    auto idx = 0;
    for (int col = 0; col < m.size.col; col++) {
        for (int row = 0; row < m.size.row; row++) {
            out(row, col) = m.data[idx++];
        }
    }
    return out;
};

inline auto to_imsym(std::unordered_map<sym::Key, Eigen::MatrixXd> covariances) {
    auto out = immer::map<imsym::key::key_t, dense_matrix_t>{};
    for (const auto& [k, v] : covariances) {
        out = std::move(out).set(imsym::key::to(k), to_imsym(v));
    }
    return out;
};

template<typename Scalar>
inline auto to_imsym(const Eigen::SparseMatrix<Scalar>& sparse_matrix) -> sparse_matrix_t {
    sparse_matrix_t out;
    out.size = {sparse_matrix.rows(), sparse_matrix.cols()};

    for (int k = 0; k < sparse_matrix.outerSize(); ++k) {
        for (typename Eigen::SparseMatrix<Scalar>::InnerIterator it(sparse_matrix, k); it; ++it) {
            long row = it.row();
            long col = it.col();
            double value = it.value();

            // Insert the value into the immer::map with the 2D index (row, col) as the key
            out.data = std::move(out.data).set({row, col}, value);
        }
    }

    return out;
};

template<typename Scalar>
inline auto to_imsym_matrix(const Eigen::Map<const Eigen::SparseMatrix<Scalar>>& sparse_matrix)
    -> sparse_matrix_t {
    const Eigen::SparseMatrix<Scalar>& m = sparse_matrix;
    return to_imsym(m);
};

template<typename Scalar>
inline auto to_imsym_matrix(const Eigen::Map<const Eigen::MatrixX<Scalar>>& dense_matrix)
    -> dense_matrix_t {
    const Eigen::MatrixX<Scalar>& m = dense_matrix;
    return to_imsym(m);
};

inline auto to_imsym(const Eigen::VectorXd& v) {
    return immer::vector<double>{v.begin(), v.end()};
};

inline auto to_imsym(const Eigen::VectorXi& v) {
    return immer::vector<size_t>{v.begin(), v.end()};
};

/*
inline auto to_imsym(const ::eigen_lcm::MatrixXd& v) {
    return to_imsym(v);
};
*/

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
    return sparse_linearization_t{};
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

template<typename MatrixType>
inline auto to_imsym(const sym::OptimizationStats<MatrixType>& opt_stats) -> optimization_stats_t {
    auto iterations = immer::vector<optimization_iteration_t>{};
    for (const auto& iter : opt_stats.iterations) {
        auto jacobian = to_imsym_matrix(opt_stats.JacobianView(iter));
        auto imsym_iter = to_imsym(iter);
        imsym_iter.jacobian = jacobian;
        iterations = std::move(iterations).push_back(imsym_iter);
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

inline auto jacobian(const optional<optimization_iteration_t>& iteration)
    -> optional<imsym::matrix_t> {
    if (not iteration.has_value()) {
        return {};
    }
    return iteration->jacobian;
};

inline auto best_iteration(const detailed_internals_t& internals)
    -> optional<imsym::optimization_iteration_t> {
    auto best_idx = internals.optimization_stats.best_index;
    if (internals.optimization_stats.iterations.size() <= best_idx) {
        return {};
    }
    return internals.optimization_stats.iterations.at(best_idx);
};
}   // namespace imsym

