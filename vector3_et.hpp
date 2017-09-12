#if !defined VECTOR3_ET_HPP
#define VECTOR3_ET_HPP

#include <iostream>
#include <cmath>
#include <initializer_list>
#include <type_traits>

template <typename T>
class isVector3 final {
  template <typename U>
  static auto check_x(U v) -> decltype(v.x(), std::true_type());
  static auto check_x(...) -> decltype(std::false_type());

  template <typename U>
  static auto check_y(U v) -> decltype(v.y(), std::true_type());
  static auto check_y(...) -> decltype(std::false_type());

  template <typename U>
  static auto check_z(U v) -> decltype(v.z(), std::true_type());
  static auto check_z(...) -> decltype(std::false_type());
public:
  typedef decltype(check_x(std::declval<T>())) exist_x;
  typedef decltype(check_y(std::declval<T>())) exist_y;
  typedef decltype(check_z(std::declval<T>())) exist_z;

  static const bool value = exist_x::value && exist_y::value && exist_z::value;
};

template <typename T, class Vec3Enabled = void>
struct Vector3;

template <typename T>
struct Vector3<T, typename std::enable_if<std::is_floating_point<T>::value>::type> final {
  T x_, y_, z_;

  struct Add {
    static T apply(const T l, const T r) {
      return l + r;
    }
  };

  struct Sub {
    static T apply(const T l, const T r) {
      return l - r;
    }
  };

  struct Mul {
    static T apply(const T l, const T r) {
      return l * r;
    }
  };

  struct Cross {};

  struct Div {
    static T apply(const T l, const T r) {
      return l / r;
    }
  };

  template <class L, class Op, class R>
  struct Expression {
    const L& l_;
    const R& r_;
    Expression(const L& l, const R& r) : l_(l), r_(r) {}

    T x() const {
      return Op::apply(l_.x(), r_.x());
    }

    T y() const {
      return Op::apply(l_.y(), r_.y());
    }

    T z() const {
      return Op::apply(l_.z(), r_.z());
    }
  };

  template <class L, class R>
  struct Expression<L, Cross, R> {
    const L& l_;
    const R& r_;
    Expression(const L& l, const R& r) : l_(l), r_(r) {}

    T x() const {
      return Sub::apply(Mul::apply(l_.y(), r_.z()), Mul::apply(l_.z(), r_.y()));
    }

    T y() const {
      return Sub::apply(Mul::apply(l_.z(), r_.x()), Mul::apply(l_.x(), r_.z()));
    }

    T z() const {
      return Sub::apply(Mul::apply(l_.x(), r_.y()), Mul::apply(l_.y(), r_.x()));
    }
  };

  template <class Op, class R>
  struct Expression<T, Op, R> {
    const T& l_;
    const R& r_;
    Expression(const T& l, const R& r) : l_(l), r_(r) {}

    T x() const {
      return Op::apply(l_, r_.x());
    }

    T y() const {
      return Op::apply(l_, r_.y());
    }

    T z() const {
      return Op::apply(l_, r_.z());
    }
  };

  template <class L, class Op>
  struct Expression<L, Op, T> {
    const L& l_;
    const T& r_;
    Expression(const L& l, const T& r) : l_(l), r_(r) {}

    T x() const {
      return Op::apply(l_.x(), r_);
    }

    T y() const {
      return Op::apply(l_.y(), r_);
    }

    T z() const {
      return Op::apply(l_.z(), r_);
    }
  };

  Vector3(void) : x_(0), y_(0), z_(0) {}
  explicit Vector3(const T c) : x_(c), y_(c), z_(c) {}
  explicit Vector3(const T (&a)[3]) : x_(a[0]), y_(a[1]), z_(a[2]) {}
  explicit Vector3(const T* a) : x_(a[0]), y_(a[1]), z_(a[2]) {}
  Vector3(const T x0, const T y0, const T z0) : x_(x0), y_(y0), z_(z0) {}
  Vector3(const Vector3& src) : x_(src.x_), y_(src.y_), z_(src.z_) {}
  Vector3(const std::initializer_list<T> ilist) {
    assert(ilist.size() == 3);
    T* it = &x_;
    for (const auto i : ilist) *it++ = i;
  }
  template <class E>
  Vector3(const E& r) { *this = r; }

  template <class E>
    const Vector3& operator = (const E& r) {
    this->x_ = r.x();
    this->y_ = r.y();
    this->z_ = r.z();
    return *this;
  }

  T& x() { return x_; }
  const T& x() const { return x_; }
  T& y() { return y_; }
  const T& y() const { return y_; }
  T& z() { return z_; }
  const T& z() const { return z_; }

  const Vector3 operator - () const {
    return Vector3(-x_, -y_, -z_);
  }

  T& operator [] (const int i) {
    return (&x_)[i];
  }

  const T& operator [] (const int i) const {
    return (&x_)[i];
  }

  void clear(void) {
    x_ = y_ = z_ = 0.0;
  }

  T norm2(void) const {
    return x_*x_ + y_*y_ + z_*z_;
  }

  T norm(void) const {
    return std::sqrt(norm2());
  }

  void normalize(void) {
    const auto r = norm();
    x_ /= r;
    y_ /= r;
    z_ /= r;
  }

  const Vector3& operator += (const Vector3& rhs) {
    this->x_ += rhs.x_;
    this->y_ += rhs.y_;
    this->z_ += rhs.z_;
    return *this;
  }

  const Vector3& operator -= (const Vector3& rhs) {
    this->x_ -= rhs.x_;
    this->y_ -= rhs.y_;
    this->z_ -= rhs.z_;
    return *this;
  }

  const Vector3& operator *= (const T c) {
    this->x_ *= c;
    this->y_ *= c;
    this->z_ *= c;
    return *this;
  }

  const Vector3& operator /= (const T c) {
    this->x_ /= c;
    this->y_ /= c;
    this->z_ /= c;
    return *this;
  }

  friend std::ostream& operator << (std::ostream& ost, const Vector3& v) {
    ost << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
    return ost;
  }

  template <class L, class R>
  friend Expression<L, Add, R> operator + (const L& lhs, const R& rhs) {
    return Expression<L, Add, R>(lhs, rhs);
  }

  template <class L, class R>
  friend Expression<L, Sub, R> operator - (const L& lhs, const R& rhs) {
    return Expression<L, Sub, R>(lhs, rhs);
  }

  template <class L, class R>
  friend Expression<L, Mul, R> operator * (const L& lhs, const R& rhs) {
    return Expression<L, Mul, R>(lhs, rhs);
  }

  template <class L, class R>
  friend Expression<L, Div, R> operator / (const L& lhs, const R& rhs) {
    return Expression<L, Div, R>(lhs, rhs);
  }

  // cross product
  template <class L, class R>
  friend Expression<L, Cross, R> operator ^ (const L& lhs, const R& rhs) {
    return Expression<L, Cross, R>(lhs, rhs);
  }

  // inner product
  template <class L, class R>
  friend T dot(const L& lhs, const R& rhs) {
    return lhs.x()*rhs.x() + lhs.y()*rhs.y() + lhs.z()*rhs.z();
  }

  template <class E>
  friend Vector3 normalize(const E& rhs) {
    Vector3 eval = Vector3(rhs);
    eval.normalize();
    return eval;
  }
};

#endif
