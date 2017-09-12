#include "gtest/gtest.h"
#include "../vector2_et.hpp"

using namespace LocalStress;

typedef Vector2<double> dvec2;

TEST(Constructor, test0) {
  double array[] {0, 2.0};
  dvec2 d(array);
  ASSERT_EQ(d.x(), 0);
  ASSERT_EQ(d.y(), 2.0);
}

TEST(Constructor, test1) {
  dvec2 d;
  ASSERT_EQ(d.x(), 0);
  ASSERT_EQ(d.y(), 0);
}

TEST(Constructor, test2) {
  dvec2 d(10.0);
  ASSERT_EQ(d.x(), 10.0);
  ASSERT_EQ(d.y(), 10.0);
}

TEST(Constructor, test3) {
  dvec2 d(12.0, 13.0);
  ASSERT_EQ(d.x(), 12.0);
  ASSERT_EQ(d.y(), 13.0);
}

TEST(Constructor, test4) {
  dvec2 d0(12.0, 13.0);
  dvec2 d1(d0);
  ASSERT_EQ(d1.x(), 12.0);
  ASSERT_EQ(d1.y(), 13.0);
}

TEST(Constructor, test5) {
  dvec2 d0 {1.0, 2.0};
  ASSERT_EQ(d0.x(), 1.0);
  ASSERT_EQ(d0.y(), 2.0);
}

TEST(Copy, test0) {
  dvec2 d0(12.0, 13.0);
  dvec2 d1;
  d1 = d0;
  ASSERT_EQ(d1.x(), 12.0);
  ASSERT_EQ(d1.y(), 13.0);
}

TEST(Operator, element_at) {
  dvec2 d(12.0, 13.0);
  ASSERT_EQ(d[0], 12.0);
  ASSERT_EQ(d[1], 13.0);

  d[0] = 1.0;
  d[1] = 2.0;
  ASSERT_EQ(d[0], 1.0);
  ASSERT_EQ(d[1], 2.0);
}

TEST(Utils, clear) {
  dvec2 d(12.0, 14.0);
  d.clear();

  ASSERT_EQ(d.x(), 0);
  ASSERT_EQ(d.y(), 0);
}

TEST(Operator, add) {
  dvec2 d0(2.0, 4.0);
  dvec2 d1(3.0, 4.0);

  auto d2 = d0 + d1;
  ASSERT_EQ(d2.x(), 5.0);
  ASSERT_EQ(d2.y(), 8.0);
}

TEST(Operator, sub) {
  dvec2 d0(2.0, 4.0);
  dvec2 d1(3.0, 4.0);

  const auto d2 = d0 - d1;
  ASSERT_EQ(d2.x(), -1.0);
  ASSERT_EQ(d2.y(), 0.0);
}

TEST(Operator, addeq) {
  dvec2 d0(2.0, 4.0);
  dvec2 d1(3.0, 4.0);

  d1 += d0;
  ASSERT_EQ(d1.x(), 5.0);
  ASSERT_EQ(d1.y(), 8.0);
}

TEST(Operator, subeq) {
  dvec2 d0(2.0, 4.0);
  dvec2 d1(3.0, 4.0);

  d0 -= d1;
  ASSERT_EQ(d0.x(), -1.0);
  ASSERT_EQ(d0.y(), 0.0);
}

TEST(Operator, mul_prod) {
  dvec2 d0(2.0, 4.0);
  dvec2 d1(3.0, 4.0);
  const double c = static_cast<Scalar<double>>(dot(d0, d1));
  ASSERT_EQ(c, 22.0);
}

TEST(Operator, mul0) {
  dvec2 d0(2.0, 4.0);
  dvec2 d1 = d0 * 2.0;
  ASSERT_EQ(d1.x(), d0.x() * 2.0);
  ASSERT_EQ(d1.y(), d0.y() * 2.0);
}

TEST(Operator, mul1) {
  dvec2 d0(2.0, 4.0);
  dvec2 d1 = 2.0 * d0;
  ASSERT_EQ(d1.x(), 2.0 * d0.x());
  ASSERT_EQ(d1.y(), 2.0 * d0.y());
}

TEST(Operator, mul2) {
  dvec2 d(2.0, 4.0);
  d *= 2.0;
  ASSERT_EQ(d.x(), 4.0);
  ASSERT_EQ(d.y(), 8.0);
}

TEST(Operator, div0) {
  dvec2 d0(2.0, 4.0);
  dvec2 d1 = d0 / 2.0;
  ASSERT_EQ(d1.x(), d0.x() / 2.0);
  ASSERT_EQ(d1.y(), d0.y() / 2.0);
}

TEST(Operator, div1) {
  dvec2 d0(2.0, 4.0);
  dvec2 d1 = 2.0 / d0;
  ASSERT_EQ(d1.x(), 2.0 / d0.x());
  ASSERT_EQ(d1.y(), 2.0 / d0.y());
}

TEST(Operator, div2) {
  dvec2 d(2.0, 4.0);
  d /= 2.0;
  ASSERT_EQ(d.x(), 1.0);
  ASSERT_EQ(d.y(), 2.0);
}

TEST(Utils, normalize0) {
  dvec2 d0(2.0, 4.0);
  dvec2 d1 = normalize(d0);
  ASSERT_DOUBLE_EQ(d1.norm(), 1.0);
  d0.normalize();
  ASSERT_DOUBLE_EQ(d0.norm(), 1.0);
  dvec2 d2(2.0, 4.0);
  ASSERT_DOUBLE_EQ(d2.norm2(), d2.x()*d2.x() + d2.y()*d2.y());
}
