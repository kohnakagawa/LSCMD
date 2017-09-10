#include "gtest/gtest.h"
#include "../ls_calculator.hpp"

using namespace LS;
constexpr double err_fp = 1.0e-13;

TEST(Constructor, test0) {
  Boundary<double> boundary(BoundaryType::PERIODIC_X, {10, 12, 14});
  boundary.setBox({1.0, 1.0, 1.0}, {11.0, 25.0, 43.0});
  const auto& m_dim = boundary.mesh_dim();
  ASSERT_EQ(m_dim[X], 10);
  ASSERT_EQ(m_dim[Y], 12);
  ASSERT_EQ(m_dim[Z], 14);

  const auto& m_len = boundary.mesh_length();
  ASSERT_EQ(m_len.x, 1.0);
  ASSERT_EQ(m_len.y, 2.0);
  ASSERT_EQ(m_len.z, 3.0);

  ASSERT_EQ(boundary.number_of_cell(), 10*12*14);

  const auto& b_len = boundary.box_length();
  ASSERT_EQ(b_len.x, 10.0);
  ASSERT_EQ(b_len.y, 24.0);
  ASSERT_EQ(b_len.z, 42.0);
}

TEST(Utils, minimum_image_XYZ) {
  Boundary<double> boundary(BoundaryType::PERIODIC_XYZ, {10, 10, 10});
  boundary.setBox({2.0, 3.0, 4.0}, {12.0, 23.0, 34.0});
  Vector3<double> r0 {2.0,  1.0,  1.0}; r0 += boundary.low();
  Vector3<double> r1 {8.0, 19.0, 29.0}; r1 += boundary.low();
  Vector3<double> dr01 = r0 - r1;
  boundary.applyMinimumImage(dr01);
  ASSERT_EQ(dr01.x, 4.0);
  ASSERT_EQ(dr01.y, 2.0);
  ASSERT_EQ(dr01.z, 2.0);
}

TEST(Utils, minimum_image_XY) {
  Boundary<double> boundary(BoundaryType::PERIODIC_XY, {10, 10, 10});
  boundary.setBox({2.0, 3.0, 4.0}, {12.0, 23.0, 34.0});
  Vector3<double> r0 {2.0,  1.0,  1.0}; r0 += boundary.low();
  Vector3<double> r1 {8.0, 19.0, 29.0}; r1 += boundary.low();
  Vector3<double> dr01 = r0 - r1;
  boundary.applyMinimumImage(dr01);
  ASSERT_EQ(dr01.x, 4.0);
  ASSERT_EQ(dr01.y, 2.0);
  ASSERT_EQ(dr01.z, -28.0);
}

TEST(Utils, minimum_image_YZ) {
  Boundary<double> boundary(BoundaryType::PERIODIC_YZ, {10, 10, 10});
  boundary.setBox({2.0, 3.0, 4.0}, {12.0, 23.0, 34.0});
  Vector3<double> r0 {2.0,  1.0,  1.0}; r0 += boundary.low();
  Vector3<double> r1 {8.0, 19.0, 29.0}; r1 += boundary.low();
  Vector3<double> dr01 = r0 - r1;
  boundary.applyMinimumImage(dr01);
  ASSERT_EQ(dr01.x, -6.0);
  ASSERT_EQ(dr01.y, 2.0);
  ASSERT_EQ(dr01.z, 2.0);
}

TEST(Utils, minimum_image_ZX) {
  Boundary<double> boundary(BoundaryType::PERIODIC_ZX, {10, 10, 10});
  boundary.setBox({2.0, 3.0, 4.0}, {12.0, 23.0, 34.0});
  Vector3<double> r0 {2.0,  1.0,  1.0}; r0 += boundary.low();
  Vector3<double> r1 {8.0, 19.0, 29.0}; r1 += boundary.low();
  Vector3<double> dr01 = r0 - r1;
  boundary.applyMinimumImage(dr01);
  ASSERT_EQ(dr01.x, 4.0);
  ASSERT_EQ(dr01.y, -18.0);
  ASSERT_EQ(dr01.z, 2.0);
}

TEST(Utils, minimum_image_X) {
  Boundary<double> boundary(BoundaryType::PERIODIC_X, {10, 10, 10});
  boundary.setBox({2.0, 3.0, 4.0}, {12.0, 23.0, 34.0});
  Vector3<double> r0 {2.0,  1.0,  1.0}; r0 += boundary.low();
  Vector3<double> r1 {8.0, 19.0, 29.0}; r1 += boundary.low();
  Vector3<double> dr01 = r0 - r1;
  boundary.applyMinimumImage(dr01);
  ASSERT_EQ(dr01.x, 4.0);
  ASSERT_EQ(dr01.y, -18.0);
  ASSERT_EQ(dr01.z, -28.0);
}

TEST(Utils, minimum_image_Y) {
  Boundary<double> boundary(BoundaryType::PERIODIC_Y, {10, 10, 10});
  boundary.setBox({2.0, 3.0, 4.0}, {12.0, 23.0, 34.0});
  Vector3<double> r0 {2.0,  1.0,  1.0}; r0 += boundary.low();
  Vector3<double> r1 {8.0, 19.0, 29.0}; r1 += boundary.low();
  Vector3<double> dr01 = r0 - r1;
  boundary.applyMinimumImage(dr01);
  ASSERT_EQ(dr01.x, -6.0);
  ASSERT_EQ(dr01.y, 2.0);
  ASSERT_EQ(dr01.z, -28.0);
}

TEST(Utils, minimum_image_Z) {
  Boundary<double> boundary(BoundaryType::PERIODIC_Z, {10, 10, 10});
  boundary.setBox({2.0, 3.0, 4.0}, {12.0, 23.0, 34.0});
  Vector3<double> r0 {2.0,  1.0,  1.0}; r0 += boundary.low();
  Vector3<double> r1 {8.0, 19.0, 29.0}; r1 += boundary.low();
  Vector3<double> dr01 = r0 - r1;
  boundary.applyMinimumImage(dr01);
  ASSERT_EQ(dr01.x, -6.0);
  ASSERT_EQ(dr01.y, -18.0);
  ASSERT_EQ(dr01.z, 2.0);
}

TEST(Utils, minimum_image_fixed) {
  Boundary<double> boundary(BoundaryType::FIXED, {10, 10, 10});
  boundary.setBox({2.0, 3.0, 4.0}, {12.0, 23.0, 34.0});
  Vector3<double> r0 {2.0,  1.0,  1.0}; r0 += boundary.low();
  Vector3<double> r1 {8.0, 19.0, 29.0}; r1 += boundary.low();
  Vector3<double> dr01 = r0 - r1;
  boundary.applyMinimumImage(dr01);
  ASSERT_EQ(dr01.x, -6.0);
  ASSERT_EQ(dr01.y, -18.0);
  ASSERT_EQ(dr01.z, -28.0);
}

TEST(Utils, adjust_boundary) {
  Boundary<double> boundary(BoundaryType::PERIODIC_Z, {10, 10, 10});
  boundary.setBox({2.0, 3.0, 4.0}, {12.0, 23.0, 34.0});

  Vector3<double> r0 {4.0, 10.0, 0.0};
  boundary.adjustBoundary(r0);
  ASSERT_EQ(r0.x, 4.0);
  ASSERT_EQ(r0.y, 10.0);
  ASSERT_EQ(r0.z, 30.0);

  Vector3<double> r1 {4.0, 10.0, 36.0};
  boundary.adjustBoundary(r1);
  ASSERT_EQ(r1.x, 4.0);
  ASSERT_EQ(r1.y, 10.0);
  ASSERT_EQ(r1.z, 6.0);
}

TEST(Utils, is_in_box) {
  Boundary<double> boundary(BoundaryType::PERIODIC_Z, {10, 10, 10});
  boundary.setBox({2.0, 3.0, 4.0}, {12.0, 23.0, 34.0});

  Vector3<double> r0 {0.0, 0.0, 0.0};
  Vector3<double> r1 {3.0, 10.0, 20.0};

  ASSERT_EQ(boundary.isInBox(r0), false);
  ASSERT_EQ(boundary.isInBox(r1), true);
}

TEST(Utils, get_cellposition_hash) {
  Boundary<double> boundary(BoundaryType::PERIODIC_XYZ, {10, 12, 14});
  boundary.setBox({1.0, 1.0, 1.0}, {11.0, 25.0, 43.0}); // mesh_len = {1.0, 2.0, 3.0};

  Vector3<double> r0 {1.5, 16.0, 32.0}; // {0, 7, 10}
  ASSERT_EQ(boundary.getCellPositionHash(r0), 1270);
}

TEST(Utils, get_lineratio0) {
  Boundary<double> boundary(BoundaryType::PERIODIC_XYZ, {10, 10, 10});
  boundary.setBox({2.0, 3.0, 4.0}, {12.0, 23.0, 34.0});

  Vector3<double> r0 {9.5, 1.0, 1.5};
  Vector3<double> r1 {2.5, 5.0, 25.5};
  r0 += boundary.low();
  r1 += boundary.low();
  Vector3<double> dr01 = r0 - r1; boundary.applyMinimumImage(dr01);
  Vector3<double> dr10 = r1 - r0; boundary.applyMinimumImage(dr10);

  auto lratios0 = boundary.getDividedLineRatio(r1, dr01);
  auto lratios1 = boundary.getDividedLineRatio(r0, dr10);

  std::sort(lratios0.begin(), lratios0.end());
  std::sort(lratios1.begin(), lratios1.end());

  ASSERT_EQ(lratios0.size(), lratios1.size());
  const std::size_t num_divided = lratios1.size();
  for (std::size_t i = 0; i < num_divided; i++) {
    ASSERT_NEAR(lratios0[i].first, lratios1[i].first, err_fp);
    ASSERT_NEAR(lratios0[i].second, lratios1[i].second, err_fp);
  }
}

TEST(Utils, get_lineratio1) {
  Boundary<double> boundary(BoundaryType::PERIODIC_XYZ, {10, 11, 12});
  boundary.setBox({2.0, 3.0, 4.0}, {12.0, 25.0, 40.0});

  Vector3<double> mesh_vec = boundary.mesh_length();
  mesh_vec.x *= -2.0;
  mesh_vec.y *= -1.0;
  mesh_vec.z *= -4.0;

  Vector3<double> r0 {0.5, 1.0, 1.5};
  Vector3<double> r1 = mesh_vec + r0;

  r0 += boundary.low();
  r1 += boundary.low();
  Vector3<double> dr01 = r0 - r1; boundary.applyMinimumImage(dr01);

  auto lratios0 = boundary.getDividedLineRatio(r1, dr01);
  double sum = 0.0;
  for (const auto lr : lratios0) {
    sum += lr.second;
    ASSERT_NEAR(lr.second, 0.125, err_fp);
  }
  ASSERT_NEAR(sum, 1.0, err_fp);

  ASSERT_EQ(lratios0.size(), 8);
  ASSERT_EQ(lratios0[7].first, 0);
  ASSERT_EQ(lratios0[6].first, 1210);
  ASSERT_EQ(lratios0[5].first, 1210 + 9);
  ASSERT_EQ(lratios0[4].first, 1210 + 9 - 110);
  ASSERT_EQ(lratios0[3].first, 1210 + 9 - 110 + 100);
  ASSERT_EQ(lratios0[2].first, 1210 + 9 - 110 + 100 - 110);
  ASSERT_EQ(lratios0[1].first, 1210 + 9 - 110 + 100 - 110 - 1);
  ASSERT_EQ(lratios0[0].first, 1210 + 9 - 110 + 100 - 110 - 1 - 110);
}
