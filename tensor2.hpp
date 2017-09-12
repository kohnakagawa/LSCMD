#if !defined TENSOR3_HPP
#define TENSOR3_HPP

#include <iostream>
#include <cassert>
#include <initializer_list>
#include "vector2.hpp"

namespace LocalStress {
  template <typename T>
  struct Tensor2 final {
    T xx, xy;
    T yx, yy;

    Tensor2(void) :
      xx(0), xy(0),
      yx(0), yy(0) {}
    explicit Tensor2(const T c) :
      xx(c), xy(c),
      yx(c), yy(c) {}
    Tensor2(const T xx_, const T xy_,
            const T yx_, const T yy_) :
      xx(xx_), xy(xy_),
      yx(yx_), yy(yy_) {}
    Tensor2(const Tensor2<T>& src) :
      xx(src.xx), xy(src.xy),
      yx(src.yx), yy(src.yy) {}
    Tensor2(const std::initializer_list<T> ilist) {
      assert(ilist.size() == 4);
      T* it = &xx;
      for (const auto i : ilist) *it++ = i;
    }

    const Tensor2& operator = (const Tensor2& src) {
      xx = src.xx; xy = src.xy;
      yx = src.yx; yy = src.yy;
      return *this;
    }

    const Tensor2 operator + (const Tensor2& rhs) const {
      return Tensor2(xx + rhs.xx, xy + rhs.xy,
                     yx + rhs.yx, yy + rhs.yy);
    }

    const Tensor2& operator += (const Tensor2& rhs) {
      *this = (*this) + rhs;
      return *this;
    }

    const Tensor2 operator - (const Tensor2& rhs) const {
      return Tensor2(xx - rhs.xx, xy - rhs.xy,
                     yx - rhs.yx, yy - rhs.yy);
    }

    const Tensor2& operator -= (const Tensor2& rhs) {
      *this = (*this) - rhs;
      return *this;
    }

    const Tensor2 operator * (const T c) const {
      return Tensor2(xx * c, xy * c,
                     yx * c, yy * c);
    }

    friend const Tensor2 operator * (const T c, const Tensor2& rhs) {
      return Tensor2(c * rhs.xx, c * rhs.xy,
                     c * rhs.yx, c * rhs.yy);
    }

    const Tensor2& operator *= (const T c) {
      *this = (*this) * c;
      return *this;
    }

    const Tensor2 operator / (const T c) const {
      return Tensor2(xx / c, xy / c,
                     yx / c, yy / c);
    }

    const Tensor2& operator /= (const T c) {
      *this = (*this) / c;
      return *this;
    }

    friend std::ostream& operator << (std::ostream& ost, const Tensor2& t) {
      ost << "(";
      ost << t.xx << ", " << t.xy << ",\n";
      ost << t.yx << ", " << t.yy << ")";
      return ost;
    }

    void clear(void) {
      xx = xy = 0.0;
      yx = yy = 0.0;
    }

    T trace(void) const {
      return xx + yy;
    }

    T det(void) const {
      return xx * yy - xy * yx;
    }
  };

  template <typename T>
  static inline Tensor2<T> tensor_dot(const Vector2<T>& v0,
                                      const Vector2<T>& v1) {
    return Tensor2<T>(v0.x * v1.x, v0.x * v1.y,
                      v0.y * v1.x, v0.y * v1.y);
  }
}
#endif
