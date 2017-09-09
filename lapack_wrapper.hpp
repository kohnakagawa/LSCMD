#if !defined LAPACK_WRAPPER_HPP
#define LAPACK_WRAPPER_HPP

#include <iostream>
#include <array>
#include <lapacke.h>

namespace LocalStress {
  template <int nrows, int ncols, int nrhs, int layout = LAPACK_COL_MAJOR>
  void call_dgelsd(double* D,
                   double* b,
                   double rcond = 1.0e-12) {
    constexpr int leading = (nrows > ncols) ? nrows : ncols;

    std::array<double, nrows> s;
    int rank = -1;
    const auto info = LAPACKE_dgelsd(layout,
                                     nrows, ncols, nrhs, D, nrows,
                                     b, leading, s.data(), rcond, &rank);
    if (info > 0) {
      ERR("The algorithm computing SVD failed to converge.\nThe least squares solution could not be computed.");
    }
  }
}
#endif
