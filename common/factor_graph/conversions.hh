#pragma once
#include "common/factor_graph/types.hh"
#include "external/symforce/symforce/opt/key.h"
#include "imsym/imsym.hh"

#include <Eigen/Core>

namespace factor_graph {
inline auto to_dense_matrix(const Eigen::MatrixXd& m) -> factor_graph::dense_matrix_t {
    factor_graph::dense_matrix_t out;
    out.size = {.row = m.rows(), .col = m.cols()};
    for (int col = 0; col < m.cols(); col++) {
        for (int row = 0; row < m.rows(); row++) {
            out.data = std::move(out.data).push_back(m(row, col));
        }
    }
    return out;
};

inline auto to_eigen(const factor_graph::dense_matrix_t& m) -> Eigen::MatrixXd {
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
        out = move(out).set(imsym::key::to(k), to_dense_matrix(v));
    }
    return out;
};

}   // namespace factor_graph
