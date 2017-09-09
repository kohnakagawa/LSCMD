#if !defined TENSOR3_HPP
#define TENSOR3_HPP

#include <iostream>
#include <cassert>
#include <initializer_list>
#include "vector3.hpp"

namespace LocalStress {
  template <typename T>
  struct Tensor3 final {
    T xx, xy, xz;
    T yx, yy, yz;
    T zx, zy, zz;

    Tensor3(void) :
      xx(0), xy(0), xz(0),
      yx(0), yy(0), yz(0),
      zx(0), zy(0), zz(0) {}
    explicit Tensor3(const T c) :
      xx(c), xy(c), xz(c),
      yx(c), yy(c), yz(c),
      zx(c), zy(c), zz(c) {}
    Tensor3(const T xx_, const T xy_, const T xz_,
            const T yx_, const T yy_, const T yz_,
            const T zx_, const T zy_, const T zz_) :
      xx(xx_), xy(xy_), xz(xz_),
      yx(yx_), yy(yy_), yz(yz_),
      zx(zx_), zy(zy_), zz(zz_) {}
    Tensor3(const Tensor3<T>& src) :
      xx(src.xx), xy(src.xy), xz(src.xz),
      yx(src.yx), yy(src.yy), yz(src.yz),
      zx(src.zx), zy(src.zy), zz(src.zz) {}
    Tensor3(const std::initializer_list<T> ilist) {
      assert(ilist.size() == 9);
      T* it = &xx;
      for (const auto i : ilist) *it++ = i;
    }

    const Tensor3& operator = (const Tensor3& src) {
      xx = src.xx; xy = src.xy; xz = src.xz;
      yx = src.yx; yy = src.yy; yz = src.yz;
      zx = src.zx; zy = src.zy; zz = src.zz;
      return *this;
    }

    const Tensor3 operator + (const Tensor3& rhs) const {
      return Tensor3(xx + rhs.xx, xy + rhs.xy, xz + rhs.xz,
                     yx + rhs.yx, yy + rhs.yy, yz + rhs.yz,
                     zx + rhs.zx, zy + rhs.zy, zz + rhs.zz);
    }

    const Tensor3& operator += (const Tensor3& rhs) {
      *this = (*this) + rhs;
      return *this;
    }

    const Tensor3 operator - (const Tensor3& rhs) const {
      return Tensor3(xx - rhs.xx, xy - rhs.xy, xz - rhs.xz,
                     yx - rhs.yx, yy - rhs.yy, yz - rhs.yz,
                     zx - rhs.zx, zy - rhs.zy, zz - rhs.zz);
    }

    const Tensor3& operator -= (const Tensor3& rhs) {
      *this = (*this) - rhs;
      return *this;
    }

    const Tensor3 operator * (const T c) const {
      return Tensor3(xx * c, xy * c, xz * c,
                     yx * c, yy * c, yz * c,
                     zx * c, zy * c, zz * c);
    }

    friend const Tensor3 operator * (const T c, const Tensor3& rhs) {
      return Tensor3(c * rhs.xx, c * rhs.xy, c * rhs.xz,
                     c * rhs.yx, c * rhs.yy, c * rhs.yz,
                     c * rhs.zx, c * rhs.zy, c * rhs.zz);
    }

    const Tensor3& operator *= (const T c) {
      *this = (*this) * c;
      return *this;
    }

    const Tensor3 operator / (const T c) const {
      return Tensor3(xx / c, xy / c, xz / c,
                     yx / c, yy / c, yz / c,
                     zx / c, zy / c, zz / c);
    }

    const Tensor3& operator /= (const T c) {
      *this = (*this) / c;
      return *this;
    }

    friend std::ostream& operator << (std::ostream& ost, const Tensor3& t) {
      ost << "(";
      ost << t.xx << ", " << t.xy << ", " << t.xz << ",\n";
      ost << t.yx << ", " << t.yy << ", " << t.yz << ",\n";
      ost << t.zx << ", " << t.zy << ", " << t.zz << ")";
      return ost;
    }

    void clear(void) {
      xx = xy = xz = 0.0;
      yx = yy = yz = 0.0;
      zx = zy = zz = 0.0;
    }
  };

  template <typename T>
  static inline Tensor3<T> tensor_dot(const Vector3<T>& v0,
                                      const Vector3<T>& v1) {
    return Tensor3<T>(v0.x * v1.x, v0.x * v1.y, v0.x * v1.z,
                      v0.y * v1.x, v0.y * v1.y, v0.y * v1.z,
                      v0.z * v1.x, v0.z * v1.y, v0.z * v1.z);
  }
}
#endif
