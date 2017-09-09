#include "gtest/gtest.h"
#include "../ls_calculator.hpp"

#include <random>

using namespace LS;

TEST(Fdecompose, three_body) {
  constexpr std::size_t num_body = 3;
  constexpr double err_fp = 1.0e-13;

  Vec<double> r0 {0.0, 0.0, 0.0};
  Vec<double> r1 {0.0, 1.0, 2.0};
  Vec<double> r2 {0.0, 4.0, 5.0};

  std::array<Vec<double>, num_body * (num_body - 1) / 2> dr {r0 - r1, r1 - r2, r2 - r0};

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

  std::array<Vec<double>, num_body> F {
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


