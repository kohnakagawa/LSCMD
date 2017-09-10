#if !defined F_DECOMPOSER_HPP
#define F_DECOMPOSER_HPP

// #include "lapack_wrapper.hpp"

namespace LocalStress {
#define DECOMPOSE_FORCE_FUNC(N)                                         \
  template <typename T>                                                 \
  auto decomposeForce(const std::array<Vec<T>, N>& F,                   \
                      const std::array<Vec<T>, N*(N-1)/2>& dr) -> remove_reference_t<decltype(dr)> \

  // NOTE: Central Force Decomposition for three body force
  // drij = ri - rj;
  // dr = {dr01, dr12, dr20};
  // dF = {dF01, dF12, dF20};
  DECOMPOSE_FORCE_FUNC(3) {
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

  // TODO: support four body force
  // NOTE: Central Force Decomposition for four body force
  // drij = ri - rj;
  // dr = {dr01, };
  // dF = {};
  /*DECOMPOSE_FORCE_FUNC(4) {
    using dr_pair_t = remove_const_t<remove_reference_t<decltype(dr)>>;

    constexpr int nrows = 12, ncols = 6, nrhs = 1;

    std::array<T, nrows * ncols> D;
    std::array<T, nrows> b;
    }*/

#undef DECOMPOSE_FORCE_FUNC
}
#endif
