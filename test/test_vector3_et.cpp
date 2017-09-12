#include "gtest/gtest.h"
#include "../vector3_et.hpp"

#include <array>

typedef Vector3<double> dvec3;

TEST(Constructor, test0) {
  double array[] {0, 2.0, 4.0};
  dvec3 d(array);
  ASSERT_EQ(d.x(), 0);
  ASSERT_EQ(d.y(), 2.0);
  ASSERT_EQ(d.z(), 4.0);
}

TEST(Constructor, test1) {
  dvec3 d;
  ASSERT_EQ(d.x(), 0);
  ASSERT_EQ(d.y(), 0);
  ASSERT_EQ(d.z(), 0);
}

TEST(Constructor, test2) {
  dvec3 d(10.0);
  ASSERT_EQ(d.x(), 10.0);
  ASSERT_EQ(d.y(), 10.0);
  ASSERT_EQ(d.z(), 10.0);
}

TEST(Constructor, test3) {
  dvec3 d(12.0, 13.0, 14.0);
  ASSERT_EQ(d.x(), 12.0);
  ASSERT_EQ(d.y(), 13.0);
  ASSERT_EQ(d.z(), 14.0);
}

TEST(Constructor, test4) {
  dvec3 d0(12.0, 13.0, 14.0);
  dvec3 d1(d0);
  ASSERT_EQ(d1.x(), 12.0);
  ASSERT_EQ(d1.y(), 13.0);
  ASSERT_EQ(d1.z(), 14.0);
}

TEST(Constructor, test5) {
  dvec3 d0 {1.0, 2.0, 3.0};
  ASSERT_EQ(d0.x(), 1.0);
  ASSERT_EQ(d0.y(), 2.0);
  ASSERT_EQ(d0.z(), 3.0);
}

TEST(Copy, test0) {
  dvec3 d0(12.0, 13.0, 14.0);
  dvec3 d1;
  d1 = d0;
  ASSERT_EQ(d1.x(), 12.0);
  ASSERT_EQ(d1.y(), 13.0);
  ASSERT_EQ(d1.z(), 14.0);
}

TEST(Operator, element_at) {
  dvec3 d(12.0, 13.0, 14.0);
  ASSERT_EQ(d[0], 12.0);
  ASSERT_EQ(d[1], 13.0);
  ASSERT_EQ(d[2], 14.0);

  d[0] = 1.0;
  d[1] = 2.0;
  d[2] = 3.0;
  ASSERT_EQ(d[0], 1.0);
  ASSERT_EQ(d[1], 2.0);
  ASSERT_EQ(d[2], 3.0);
}

TEST(Utils, clear) {
  dvec3 d(12.0, 14.0, 15.0);
  d.clear();

  ASSERT_EQ(d.x(), 0);
  ASSERT_EQ(d.y(), 0);
  ASSERT_EQ(d.z(), 0);
}

TEST(Operator, add) {
  dvec3 d0(2.0, 4.0, 1.0);
  dvec3 d1(3.0, 4.0, -1.0);

  dvec3 d2 = d0 + d1;
  ASSERT_EQ(d2.x(), 5.0);
  ASSERT_EQ(d2.y(), 8.0);
  ASSERT_EQ(d2.z(), 0.0);
}

TEST(Operator, sub) {
  dvec3 d0(2.0, 4.0, 1.0);
  dvec3 d1(3.0, 4.0, -1.0);

  const dvec3 d2 = d0 - d1;
  ASSERT_EQ(d2.x(), -1.0);
  ASSERT_EQ(d2.y(), 0.0);
  ASSERT_EQ(d2.z(), 2.0);
}

TEST(Operator, addeq) {
  dvec3 d0(2.0, 4.0, 1.0);
  dvec3 d1(3.0, 4.0, -1.0);

  d1 += d0;
  ASSERT_EQ(d1.x(), 5.0);
  ASSERT_EQ(d1.y(), 8.0);
  ASSERT_EQ(d1.z(), 0.0);
}

TEST(Operator, subeq) {
  dvec3 d0(2.0, 4.0, 1.0);
  dvec3 d1(3.0, 4.0, -1.0);

  d0 -= d1;
  ASSERT_EQ(d0.x(), -1.0);
  ASSERT_EQ(d0.y(), 0.0);
  ASSERT_EQ(d0.z(), 2.0);
}

TEST(Operator, mul_prod) {
  dvec3 d0(2.0, 4.0, 1.0);
  dvec3 d1(3.0, 4.0, -1.0);
  const double c = dot(d0, d1);
  ASSERT_EQ(c, 21.0);
}

TEST(Operator, mul0) {
  dvec3 d0(2.0, 4.0, 1.0);
  dvec3 d1 = d0 * 2.0;
  ASSERT_EQ(d1.x(), d0.x() * 2.0);
  ASSERT_EQ(d1.y(), d0.y() * 2.0);
  ASSERT_EQ(d1.z(), d0.z() * 2.0);
}

TEST(Operator, mul1) {
  dvec3 d0(2.0, 4.0, 1.0);
  dvec3 d1 = 2.0 * d0;
  ASSERT_EQ(d1.x(), 2.0 * d0.x());
  ASSERT_EQ(d1.y(), 2.0 * d0.y());
  ASSERT_EQ(d1.z(), 2.0 * d0.z());
}

TEST(Operator, mul2) {
  dvec3 d(2.0, 4.0, 1.0);
  d *= 2.0;
  ASSERT_EQ(d.x(), 4.0);
  ASSERT_EQ(d.y(), 8.0);
  ASSERT_EQ(d.z(), 2.0);
}

TEST(Operator, div0) {
  dvec3 d0(2.0, 4.0, 1.0);
  dvec3 d1 = d0 / 2.0;
  ASSERT_EQ(d1.x(), d0.x() / 2.0);
  ASSERT_EQ(d1.y(), d0.y() / 2.0);
  ASSERT_EQ(d1.z(), d0.z() / 2.0);
}

TEST(Operator, div1) {
  dvec3 d0(2.0, 4.0, 1.0);
  dvec3 d1 = 2.0 / d0;
  ASSERT_EQ(d1.x(), 2.0 / d0.x());
  ASSERT_EQ(d1.y(), 2.0 / d0.y());
  ASSERT_EQ(d1.z(), 2.0 / d0.z());
}

TEST(Operator, div2) {
  dvec3 d(2.0, 4.0, 1.0);
  d /= 2.0;
  ASSERT_EQ(d.x(), 1.0);
  ASSERT_EQ(d.y(), 2.0);
  ASSERT_EQ(d.z(), 0.5);
}

TEST(Utils, normalize0) {
  dvec3 d0(2.0, 4.0, 1.0);
  dvec3 d1 = normalize(d0);
  ASSERT_DOUBLE_EQ(d1.norm(), 1.0);
  d0.normalize();
  ASSERT_DOUBLE_EQ(d0.norm(), 1.0);
  dvec3 d2(2.0, 4.0, 1.0);
  ASSERT_DOUBLE_EQ(d2.norm2(), d2.x()*d2.x() + d2.y()*d2.y() + d2.z()*d2.z());
}
