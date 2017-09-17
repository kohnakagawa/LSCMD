#if !defined F_DECOMPOSER_HPP
#define F_DECOMPOSER_HPP

#include <Eigen/Core>
#include <Eigen/LU>

namespace LocalStress {
  // NOTE:
  // drij = ri - rj;
#define DECL_DECOMPOSE_FORCE_FUNC(N)                                    \
  template <typename T, int32_t num_body = N>                           \
  auto decomposeForce(const std::array<Vec<T>, N>& F,                   \
                      const std::array<Vec<T>, N*(N-1)/2>& dr) -> remove_reference_t<decltype(dr)> \

  // in  F = {F0, F1, F2}, dr = {dr01, dr12, dr20};
  // out dF = {dF01, dF12, dF20};
  DECL_DECOMPOSE_FORCE_FUNC(3) {
    using dr_pair_t = remove_const_t<remove_reference_t<decltype(dr)>>;
    const dr_pair_t dr_u {normalize(dr[0]), normalize(dr[1]), normalize(dr[2])};
    const auto dFa = F[0] * (dr_u[0] - dr_u[2]) / (1.0 - dr_u[0] * dr_u[2]);
    const auto dFb = F[1] * (dr_u[1] - dr_u[0]) / (1.0 - dr_u[1] * dr_u[0]);
    const auto dFc = F[2] * (dr_u[2] - dr_u[1]) / (1.0 - dr_u[2] * dr_u[1]);
    const auto dF01 = 0.5 * (dFa + dFb - dFc);
    const auto dF12 = 0.5 * (dFb + dFc - dFa);
    const auto dF02 = 0.5 * (dFc + dFa - dFb);
    return {dF01 * dr_u[0], dF12 * dr_u[1], dF02 * dr_u[2]};
  }

#define VEC_TO_MAT(M, i, j, v)                  \
  do {                                          \
    for (int32_t axis = 0; axis < D; axis++) {  \
      M(D * i + axis, j) = v[axis];             \
    }                                           \
  } while (0)

  // in  F = {F0, F1, F2, F3}, dr = {dr01, dr02, dr03, dr12, dr13, dr23}
  // out dF = {dF01, dF02, dF03, dF12, dF13, dF23}
  DECL_DECOMPOSE_FORCE_FUNC(4) {
    constexpr int32_t nrows = num_body * D;
    constexpr int32_t ncols = num_body * (num_body - 1) / 2;

    Eigen::Matrix<T, nrows, ncols> dr_mat;
    dr_mat.setZero();

    // F0
    VEC_TO_MAT(dr_mat, 0, 0, dr[0]);
    VEC_TO_MAT(dr_mat, 0, 1, dr[1]);
    VEC_TO_MAT(dr_mat, 0, 2, dr[2]);

    // F1
    VEC_TO_MAT(dr_mat, 1, 0, -dr[0]);
    VEC_TO_MAT(dr_mat, 1, 3, dr[3]);
    VEC_TO_MAT(dr_mat, 1, 4, dr[4]);

    // F2
    VEC_TO_MAT(dr_mat, 2, 1, -dr[1]);
    VEC_TO_MAT(dr_mat, 2, 3, -dr[3]);
    VEC_TO_MAT(dr_mat, 2, 5, dr[5]);

    // F3
    VEC_TO_MAT(dr_mat, 3, 2, -dr[2]);
    VEC_TO_MAT(dr_mat, 3, 4, -dr[4]);
    VEC_TO_MAT(dr_mat, 3, 5, -dr[5]);

    Eigen::VectorXd F_vec(nrows);
    std::copy_n(&F[0].x, nrows, F_vec.data());

    Eigen::VectorXd cf_dF = dr_mat.fullPivLu().solve(F_vec);

    return {cf_dF[0] * dr[0], cf_dF[1] * dr[1], cf_dF[2] * dr[2],
            cf_dF[3] * dr[3], cf_dF[4] * dr[4], cf_dF[5] * dr[5]};
  }

#undef VEC_TO_MAT
#undef DECL_DECOMPOSE_FORCE_FUNC
}
#endif
