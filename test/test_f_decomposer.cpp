#include "gtest/gtest.h"
#include "../ls_calculator.hpp"

#include <random>

using namespace LS;

TEST(Fdecompose, three_body) {
  constexpr std::size_t num_body = 3;
  constexpr double err_fp = 1.0e-13;

  Vector3<double> r0 {0.0, 0.0, 0.0};
  Vector3<double> r1 {0.0, 1.0, 2.0};
  Vector3<double> r2 {0.0, 4.0, 5.0};

  std::array<Vector3<double>, num_body * (num_body - 1) / 2> dr {r0 - r1, r1 - r2, r2 - r0};

  std::mt19937 mt(10);
  std::uniform_real_distribution<> urd;

  const auto f01 = urd(mt);
  const auto f12 = urd(mt);
  const auto f20 = urd(mt);

  const auto r01_u = normalize(dr[0]);
  const auto r12_u = normalize(dr[1]);
  const auto r20_u = normalize(dr[2]);

  const auto f01_vec = f01 * r01_u;
  const auto f12_vec = f12 * r12_u;
  const auto f20_vec = f20 * r20_u;

  std::array<Vector3<double>, num_body> F {
    f01_vec - f20_vec,
    f12_vec - f01_vec,
    f20_vec - f12_vec
  };

  const auto dF = decomposeForce(F, dr);

#define ASSERT_DOUBLE3_NEAR(v0, v1, err)                     \
  ASSERT_NEAR(v0.x, v1.x, err);                              \
  ASSERT_NEAR(v0.y, v1.y, err);                              \
  ASSERT_NEAR(v0.z, v1.z, err)

  ASSERT_DOUBLE3_NEAR(dF[0], f01_vec, err_fp);
  ASSERT_DOUBLE3_NEAR(dF[1], f12_vec, err_fp);
  ASSERT_DOUBLE3_NEAR(dF[2], f20_vec, err_fp);
}

TEST(Fdecompose, four_body) {
  constexpr std::size_t num_body = 4;
  constexpr std::size_t num_pair = num_body * (num_body - 1) / 2;
  constexpr double err_fp = 1.0e-13;

  Vector3<double> r0 {0.0,  0.0,  0.0};
  Vector3<double> r1 {0.0,  1.0,  2.0};
  Vector3<double> r2 {0.0,  4.0,  5.0};
  Vector3<double> r3 {0.0, -1.0, -4.0};

  std::array<Vector3<double>, num_pair> dr {
    r0 - r1, r0 - r2, r0 - r3,
    r1 - r2, r1 - r3, r2 - r3
  };

  std::mt19937 mt(10);
  std::uniform_real_distribution<> urd;

  const auto f01 = urd(mt);
  const auto f02 = urd(mt);
  const auto f03 = urd(mt);
  const auto f12 = urd(mt);
  const auto f13 = urd(mt);
  const auto f23 = urd(mt);

  const auto f01_vec = f01 * dr[0];
  const auto f02_vec = f02 * dr[1];
  const auto f03_vec = f03 * dr[2];
  const auto f12_vec = f12 * dr[3];
  const auto f13_vec = f13 * dr[4];
  const auto f23_vec = f23 * dr[5];

  const std::array<Vector3<double>, num_body> F {
    f01_vec + f02_vec + f03_vec,
    f12_vec + f13_vec - f01_vec,
    f23_vec - f02_vec - f12_vec,
    -f03_vec - f13_vec - f23_vec
  };

  const auto dF = decomposeForce(F, dr);

  const std::array<Vector3<double>, num_body> F_r {
    dF[0] + dF[1] + dF[2],
    -dF[0] + dF[3] + dF[4],
    -dF[1] - dF[3] + dF[5],
    -dF[2] - dF[4] - dF[5]
  };
  for (std::size_t i = 0; i < num_body; i++) {
    ASSERT_NEAR((F[i] - F_r[i]).norm2(), 0.0, err_fp);
  }

  // check dr[:] // dF[:]
  for (std::size_t i = 0; i < num_pair; i++) {
    const auto dr_norm = dr[i].norm();
    const auto dF_norm = dF[i].norm();
    ASSERT_NEAR(std::abs(dF[i] * dr[i]), std::abs(dr_norm * dF_norm), err_fp);
  }
}
