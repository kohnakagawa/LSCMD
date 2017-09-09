#include "gtest/gtest.h"
#include "../ls_calculator.hpp"

using namespace LS;
constexpr double err_fp = 1.0e-13;

TEST(Constructor, test0) {
  Boundary<double> boundary(BoundaryType::PERIODIC_X, {10, 12, 14});
  boundary.setBox({1.0, 1.0, 1.0}, {11.0, 11.0, 11.0});
  std::cout << boundary.mesh_length() << std::endl;
  const std::array<int32_t, D>& m_dim = boundary.mesh_dim();
  std::cout << m_dim[0] << ", " << m_dim[1] << ", " << m_dim[2] << std::endl;
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
  Boundary<double> boundary(BoundaryType::PERIODIC_XYZ, {10, 10, 10});
  boundary.setBox({2.0, 3.0, 4.0}, {12.0, 23.0, 34.0});

  Vector3<double> r0 {9.5, 1.0, 1.5};
  Vector3<double> r1 {2.7, 5.2, 25.3};
  r0 += boundary.low();
  r1 += boundary.low();
  Vector3<double> dr01 = r0 - r1; boundary.applyMinimumImage(dr01);

  auto lratios0 = boundary.getDividedLineRatio(r1, dr01);
  double sum = 0.0;
  for (const auto lr : lratios0) {
    sum += lr.second;
    std::cout << lr.second << std::endl;
  }
  ASSERT_NEAR(sum, 1.0, err_fp);
}
