#include "gtest/gtest.h"
#include "../ls_calculator.hpp"

using namespace LocalStress;

typedef Tensor3<double> dtensor;
typedef Vector3<double> dvec3;

TEST(Constructor, test0) {
  dtensor dt;
  ASSERT_EQ(dt.xx, 0);
  ASSERT_EQ(dt.xy, 0);
  ASSERT_EQ(dt.xz, 0);

  ASSERT_EQ(dt.yx, 0);
  ASSERT_EQ(dt.yy, 0);
  ASSERT_EQ(dt.yz, 0);

  ASSERT_EQ(dt.zx, 0);
  ASSERT_EQ(dt.zy, 0);
  ASSERT_EQ(dt.zz, 0);
}

TEST(Constructor, test1) {
  dtensor dt(12.0);
  ASSERT_EQ(dt.xx, 12.0);
  ASSERT_EQ(dt.xy, 12.0);
  ASSERT_EQ(dt.xz, 12.0);

  ASSERT_EQ(dt.yx, 12.0);
  ASSERT_EQ(dt.yy, 12.0);
  ASSERT_EQ(dt.yz, 12.0);

  ASSERT_EQ(dt.zx, 12.0);
  ASSERT_EQ(dt.zy, 12.0);
  ASSERT_EQ(dt.zz, 12.0);
}

TEST(Constructor, test2) {
  dtensor dt(1.0, 2.0, 3.0,
             4.0, 5.0, 6.0,
             7.0, 8.0, 9.0);

  ASSERT_EQ(dt.xx, 1.0);
  ASSERT_EQ(dt.xy, 2.0);
  ASSERT_EQ(dt.xz, 3.0);

  ASSERT_EQ(dt.yx, 4.0);
  ASSERT_EQ(dt.yy, 5.0);
  ASSERT_EQ(dt.yz, 6.0);

  ASSERT_EQ(dt.zx, 7.0);
  ASSERT_EQ(dt.zy, 8.0);
  ASSERT_EQ(dt.zz, 9.0);
}

TEST(Constructor, test3) {
  dtensor dt0(1.0, 2.0, 3.0,
              4.0, 5.0, 6.0,
              7.0, 8.0, 9.0);
  dtensor dt(dt0);

  ASSERT_EQ(dt.xx, 1.0);
  ASSERT_EQ(dt.xy, 2.0);
  ASSERT_EQ(dt.xz, 3.0);

  ASSERT_EQ(dt.yx, 4.0);
  ASSERT_EQ(dt.yy, 5.0);
  ASSERT_EQ(dt.yz, 6.0);

  ASSERT_EQ(dt.zx, 7.0);
  ASSERT_EQ(dt.zy, 8.0);
  ASSERT_EQ(dt.zz, 9.0);
}

TEST(Constructor, test4) {
  dtensor dt {
    1.0, 2.0, 3.0,
    4.0, 5.0, 6.0,
    7.0, 8.0, 9.0
  };

  ASSERT_EQ(dt.xx, 1.0);
  ASSERT_EQ(dt.xy, 2.0);
  ASSERT_EQ(dt.xz, 3.0);

  ASSERT_EQ(dt.yx, 4.0);
  ASSERT_EQ(dt.yy, 5.0);
  ASSERT_EQ(dt.yz, 6.0);

  ASSERT_EQ(dt.zx, 7.0);
  ASSERT_EQ(dt.zy, 8.0);
  ASSERT_EQ(dt.zz, 9.0);
}

TEST(Operator, copy) {
  dtensor dt0(1.0, 2.0, 3.0,
              4.0, 5.0, 6.0,
              7.0, 8.0, 9.0);
  dtensor dt = dt0;

  ASSERT_EQ(dt.xx, 1.0);
  ASSERT_EQ(dt.xy, 2.0);
  ASSERT_EQ(dt.xz, 3.0);

  ASSERT_EQ(dt.yx, 4.0);
  ASSERT_EQ(dt.yy, 5.0);
  ASSERT_EQ(dt.yz, 6.0);

  ASSERT_EQ(dt.zx, 7.0);
  ASSERT_EQ(dt.zy, 8.0);
  ASSERT_EQ(dt.zz, 9.0);
}

TEST(Operator, add) {
  dtensor dt0(1.0, 2.0, 3.0,
              4.0, 5.0, 6.0,
              7.0, 8.0, 9.0);
  dtensor dt1(3.0, 4.0, 5.0,
              3.0, 2.0, -1.0,
              5.0, 8.0, 9.0);
  dtensor dt2 = dt0 + dt1;

  ASSERT_EQ(dt2.xx, 4.0);
  ASSERT_EQ(dt2.xy, 6.0);
  ASSERT_EQ(dt2.xz, 8.0);

  ASSERT_EQ(dt2.yx, 7.0);
  ASSERT_EQ(dt2.yy, 7.0);
  ASSERT_EQ(dt2.yz, 5.0);

  ASSERT_EQ(dt2.zx, 12.0);
  ASSERT_EQ(dt2.zy, 16.0);
  ASSERT_EQ(dt2.zz, 18.0);
}

TEST(Operator, addeq) {
  dtensor dt0(1.0, 2.0, 3.0,
              4.0, 5.0, 6.0,
              7.0, 8.0, 9.0);
  dtensor dt1(3.0, 4.0, 5.0,
              3.0, 2.0, -1.0,
              5.0, 8.0, 9.0);
  dt1 += dt0;

  ASSERT_EQ(dt1.xx, 4.0);
  ASSERT_EQ(dt1.xy, 6.0);
  ASSERT_EQ(dt1.xz, 8.0);

  ASSERT_EQ(dt1.yx, 7.0);
  ASSERT_EQ(dt1.yy, 7.0);
  ASSERT_EQ(dt1.yz, 5.0);

  ASSERT_EQ(dt1.zx, 12.0);
  ASSERT_EQ(dt1.zy, 16.0);
  ASSERT_EQ(dt1.zz, 18.0);
}

TEST(Operator, sub) {
  dtensor dt0(1.0, 2.0, 3.0,
              4.0, 5.0, 6.0,
              7.0, 8.0, 9.0);
  dtensor dt1(3.0, 4.0, 5.0,
              3.0, 2.0, -1.0,
              5.0, 8.0, 9.0);
  dtensor dt2 = dt0 - dt1;

  ASSERT_EQ(dt2.xx, -2.0);
  ASSERT_EQ(dt2.xy, -2.0);
  ASSERT_EQ(dt2.xz, -2.0);

  ASSERT_EQ(dt2.yx, 1.0);
  ASSERT_EQ(dt2.yy, 3.0);
  ASSERT_EQ(dt2.yz, 7.0);

  ASSERT_EQ(dt2.zx, 2.0);
  ASSERT_EQ(dt2.zy, 0.0);
  ASSERT_EQ(dt2.zz, 0.0);
}

TEST(Operator, subeq) {
  dtensor dt0(1.0, 2.0, 3.0,
              4.0, 5.0, 6.0,
              7.0, 8.0, 9.0);
  dtensor dt1(3.0, 4.0, 5.0,
              3.0, 2.0, -1.0,
              5.0, 8.0, 9.0);
  dt0 -= dt1;

  ASSERT_EQ(dt0.xx, -2.0);
  ASSERT_EQ(dt0.xy, -2.0);
  ASSERT_EQ(dt0.xz, -2.0);

  ASSERT_EQ(dt0.yx, 1.0);
  ASSERT_EQ(dt0.yy, 3.0);
  ASSERT_EQ(dt0.yz, 7.0);

  ASSERT_EQ(dt0.zx, 2.0);
  ASSERT_EQ(dt0.zy, 0.0);
  ASSERT_EQ(dt0.zz, 0.0);
}

TEST(Operator, mul0) {
  dtensor dt0(1.0, 2.0, 3.0,
              4.0, 5.0, 6.0,
              7.0, 8.0, 9.0);
  dtensor dt1 = dt0 * 2.0;

  ASSERT_EQ(dt1.xx, dt0.xx * 2.0);
  ASSERT_EQ(dt1.xy, dt0.xy * 2.0);
  ASSERT_EQ(dt1.xz, dt0.xz * 2.0);

  ASSERT_EQ(dt1.yx, dt0.yx * 2.0);
  ASSERT_EQ(dt1.yy, dt0.yy * 2.0);
  ASSERT_EQ(dt1.yz, dt0.yz * 2.0);

  ASSERT_EQ(dt1.zx, dt0.zx * 2.0);
  ASSERT_EQ(dt1.zy, dt0.zy * 2.0);
  ASSERT_EQ(dt1.zz, dt0.zz * 2.0);
}

TEST(Operator, mul1) {
  dtensor dt0(1.0, 2.0, 3.0,
              4.0, 5.0, 6.0,
              7.0, 8.0, 9.0);
  dtensor dt1 = 2.0 * dt0;

  ASSERT_EQ(dt1.xx, 2.0 * dt0.xx);
  ASSERT_EQ(dt1.xy, 2.0 * dt0.xy);
  ASSERT_EQ(dt1.xz, 2.0 * dt0.xz);

  ASSERT_EQ(dt1.yx, 2.0 * dt0.yx);
  ASSERT_EQ(dt1.yy, 2.0 * dt0.yy);
  ASSERT_EQ(dt1.yz, 2.0 * dt0.yz);

  ASSERT_EQ(dt1.zx, 2.0 * dt0.zx);
  ASSERT_EQ(dt1.zy, 2.0 * dt0.zy);
  ASSERT_EQ(dt1.zz, 2.0 * dt0.zz);
}

TEST(Operator, muleq) {
  dtensor dt0(1.0, 2.0, 3.0,
              4.0, 5.0, 6.0,
              7.0, 8.0, 9.0);
  dt0 *= 2.0;

  ASSERT_EQ(dt0.xx, 2.0);
  ASSERT_EQ(dt0.xy, 4.0);
  ASSERT_EQ(dt0.xz, 6.0);

  ASSERT_EQ(dt0.yx, 8.0);
  ASSERT_EQ(dt0.yy, 10.0);
  ASSERT_EQ(dt0.yz, 12.0);

  ASSERT_EQ(dt0.zx, 14.0);
  ASSERT_EQ(dt0.zy, 16.0);
  ASSERT_EQ(dt0.zz, 18.0);
}

TEST(Operator, div) {
  dtensor dt0(1.0, 2.0, 3.0,
              4.0, 5.0, 6.0,
              7.0, 8.0, 9.0);
  dtensor dt1 = dt0 / 2.0;

  ASSERT_EQ(dt1.xx, dt0.xx / 2.0);
  ASSERT_EQ(dt1.xy, dt0.xy / 2.0);
  ASSERT_EQ(dt1.xz, dt0.xz / 2.0);

  ASSERT_EQ(dt1.yx, dt0.yx / 2.0);
  ASSERT_EQ(dt1.yy, dt0.yy / 2.0);
  ASSERT_EQ(dt1.yz, dt0.yz / 2.0);

  ASSERT_EQ(dt1.zx, dt0.zx / 2.0);
  ASSERT_EQ(dt1.zy, dt0.zy / 2.0);
  ASSERT_EQ(dt1.zz, dt0.zz / 2.0);
}

TEST(Operator, diveq) {
  dtensor dt0(1.0, 2.0, 3.0,
              4.0, 5.0, 6.0,
              7.0, 8.0, 9.0);
  dt0 /= 2.0;

  ASSERT_EQ(dt0.xx, 0.5);
  ASSERT_EQ(dt0.xy, 1.0);
  ASSERT_EQ(dt0.xz, 1.5);

  ASSERT_EQ(dt0.yx, 2.0);
  ASSERT_EQ(dt0.yy, 2.5);
  ASSERT_EQ(dt0.yz, 3.0);

  ASSERT_EQ(dt0.zx, 3.5);
  ASSERT_EQ(dt0.zy, 4.0);
  ASSERT_EQ(dt0.zz, 4.5);
}

TEST(Utils, tensordot) {
  dvec3 v0(1.0, 2.0, 3.0);
  dvec3 v1(2.0, 1.0, 3.0);
  dtensor t01 = tensor_dot(v0, v1);

  ASSERT_EQ(t01.xx, v0.x * v1.x);
  ASSERT_EQ(t01.xy, v0.x * v1.y);
  ASSERT_EQ(t01.xz, v0.x * v1.z);

  ASSERT_EQ(t01.yx, v0.y * v1.x);
  ASSERT_EQ(t01.yy, v0.y * v1.y);
  ASSERT_EQ(t01.yz, v0.y * v1.z);

  ASSERT_EQ(t01.zx, v0.z * v1.x);
  ASSERT_EQ(t01.zy, v0.z * v1.y);
  ASSERT_EQ(t01.zz, v0.z * v1.z);
}
