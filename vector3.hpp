#if !defined VECTOR3_HPP
#define VECTOR3_HPP

#include <iostream>
#include <cmath>
#include <cassert>
#include <initializer_list>
#include "utils.hpp"

namespace LocalStress {
  template <typename T>
  struct Vector3 final {
    T x, y, z;

    Vector3(void) : x(0), y(0), z(0) {}
    explicit Vector3(const T c) : x(c), y(c), z(c) {}
    explicit Vector3(const T* p) : x(p[0]), y(p[1]), z(p[2]) {}
    Vector3(const T x_, const T y_, const T z_) : x(x_), y(y_), z(z_) {}
    Vector3(const Vector3& src) : x(src.x), y(src.y), z(src.z) {}
    Vector3(const std::initializer_list<T> ilist) {
      assert(ilist.size() == 3);
      T* it = &x;
      for (const auto i : ilist) *it++ = i;
    }

    const Vector3& operator = (const Vector3& src) {
      x = src.x;
      y = src.y;
      z = src.z;
      return *this;
    }

    const Vector3 operator - () const {
      return Vector3(-x, -y, -z);
    }

    T& operator [] (const int i) {
      return (&x)[i];
    }

    const T& operator [] (const int i) const {
      return (&x)[i];
    }

    void clear(void) {
      x = y = z = 0.0;
    }

    T norm2(void) const {
      return x*x + y*y + z*z;
    }

    T norm(void) const {
      return std::sqrt(norm2());
    }

    void normalize(void) {
      const auto r = norm();
      x /= r;
      y /= r;
      z /= r;
    }

    friend Vector3 normalize(const Vector3& rhs) {
      const auto r = rhs.norm();
      return rhs / r;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector3 vec) {
      os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
      return os;
    }

#define DEFINE_OP_FUNCS0(OP)                                      \
    const Vector3 operator OP (const Vector3& rhs) const {        \
      return Vector3(x OP rhs.x, y OP rhs.y, z OP rhs.z);         \
    }                                                             \
                                                                  \
    const Vector3& operator LOCAL_STRESS_CONCAT(OP, =) (const Vector3& rhs) {  \
      *this = *this OP rhs;                                       \
      return *this;                                               \
    }
    DEFINE_OP_FUNCS0(+)
      DEFINE_OP_FUNCS0(-)

      T operator * (const Vector3& rhs) const {
      return x * rhs.x + y * rhs.y + z * rhs.z;
    }

#define DEFINE_OP_FUNCS1(OP)                                            \
    friend const Vector3 operator OP (const T c, const Vector3& rhs) {  \
      return Vector3(c OP rhs.x, c OP rhs.y, c OP rhs.z);               \
    }                                                                   \
                                                                        \
    const Vector3 operator OP (const T c) const {                       \
      return Vector3(x OP c, y OP c, z OP c);                           \
    }                                                                   \
                                                                        \
    const Vector3& operator LOCAL_STRESS_CONCAT(OP, =) (const T c) {    \
      x LOCAL_STRESS_CONCAT(OP, =) c;                                   \
      y LOCAL_STRESS_CONCAT(OP, =) c;                                   \
      z LOCAL_STRESS_CONCAT(OP, =) c;                                   \
      return *this;                                                     \
    }
    DEFINE_OP_FUNCS1(*)
    DEFINE_OP_FUNCS1(/)
  };

#undef DEFINE_OP_FUNCS0
#undef DEFINE_OP_FUNCS1
}
#endif
