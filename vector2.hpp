#if !defined VECTOR2_HPP
#define VECTOR2_HPP

#include <iostream>
#include <cassert>
#include <initializer_list>
#include "utils.hpp"

namespace LocalStress {
  template <typename T>
  struct Vector2 final {
    T x, y;

    Vector2(void) : x(0), y(0) {}
    explicit Vector2(const T c) : x(c), y(c) {}
    explicit Vector2(const T* p) : x(p[0]), y(p[1]) {}
    Vector2(const T x_, const T y_) : x(x_), y(y_) {}
    Vector2(const Vector2& src) : x(src.x), y(src.y) {}
    Vector2(const std::initializer_list<T> ilist) {
      assert(ilist.size() == 2);
      T* it = &x;
      for (const auto i : ilist) *it++ = i;
    }

    const Vector2& operator = (const Vector2& src) {
      x = src.x;
      y = src.y;
      return *this;
    }

    T& operator [] (const int i) {
      return (&x)[i];
    }

    const T& operator [] (const int i) const {
      return (&x)[i];
    }

    void clear(void) {
      x = y = 0.0;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector2 vec) {
      os << "(" << vec.x << ", " << vec.y << ")";
      return os;
    }

#define DEFINE_OP_FUNCS0(OP)                                            \
    const Vector2 operator OP (const Vector2& rhs) const {              \
      return Vector2(x OP rhs.x, y OP rhs.y);                           \
    }                                                                   \
                                                                        \
    const Vector2& operator LOCAL_STRESS_CONCAT(OP, =) (const Vector2& rhs) { \
      *this = *this OP rhs;                                             \
      return *this;                                                     \
    }
    DEFINE_OP_FUNCS0(+)
    DEFINE_OP_FUNCS0(-)

    T operator * (const Vector2& rhs) const {
      return x * rhs.x + y * rhs.y;
    }

#define DEFINE_OP_FUNCS1(OP)                                            \
    friend const Vector2 operator OP (const T c, const Vector2& rhs) {  \
      return Vector2(c OP rhs.x, c OP rhs.y);                           \
    }                                                                   \
                                                                        \
    const Vector2 operator OP (const T c) const {                       \
      return Vector2(x OP c, y OP c);                                   \
    }                                                                   \
                                                                        \
    const Vector2& operator LOCAL_STRESS_CONCAT(OP, =) (const T c) {    \
      x LOCAL_STRESS_CONCAT(OP, =) c;                                   \
      y LOCAL_STRESS_CONCAT(OP, =) c;                                   \
      return *this;                                                     \
    }
    DEFINE_OP_FUNCS1(*)
    DEFINE_OP_FUNCS1(/)
  };

#undef DEFINE_OP_FUNCS0
#undef DEFINE_OP_FUNCS1
}
#endif
