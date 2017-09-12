#if !defined VECTOR2_ET_HPP
#define VECTOR2_ET_HPP

#include <iostream>
#include <cmath>
#include <initializer_list>
#include <type_traits>

namespace LocalStress {
#define CONCATENATE(x, y) x ## y
#define CONCAT(x, y) CONCATENATE(x, y)
#define STRUCT_AT(t, f) CONCAT((t)-, >f)
#define DEFINE_MEMBER_EXISTENCE_FUNCS(mfname)                           \
  template <typename U>                                                 \
  static auto CONCAT(check_, mfname)(U v) -> decltype(STRUCT_AT(&v, mfname()), std::true_type()); \
  static auto CONCAT(check_, mfname)(...) -> decltype(std::false_type()); \
  typedef decltype(CONCAT(check_, mfname)(std::declval<T>())) CONCAT(exist_, mfname)

  template <typename T>
  struct isVector2 final {
    DEFINE_MEMBER_EXISTENCE_FUNCS(x);
    DEFINE_MEMBER_EXISTENCE_FUNCS(y);
    static const bool value = exist_x::value && exist_y::value;
  };

  template <typename T>
  struct isScalar final {
    DEFINE_MEMBER_EXISTENCE_FUNCS(val);
    static const bool value = exist_val::value;
  };

#undef DEFINE_MEMBER_EXISTENCE_FUNCS
#undef CONCATENATE
#undef CONCAT
#undef STRUCT_AT

  template <typename T>
  struct Ops {
    struct Add {
      static T apply(const T l, const T r) { return l + r; }
    };
    struct Sub {
      static T apply(const T l, const T r) { return l - r; }
    };
    struct Mul {
      static T apply(const T l, const T r) { return l * r; }
    };
    struct Div {
      static T apply(const T l, const T r) { return l / r; }
    };
    struct Dot {};
    struct Cross {};
  };

  template <typename T, class ScalarEnabled = void>
  struct Scalar;

  template <typename T>
  struct Scalar<T, typename std::enable_if<std::is_floating_point<T>::value>::type> final {
    T val_;
    Scalar(const T c) : val_(c) {}
    template <class E>
      Scalar(const E& r) { *this = r; }

    template <class E>
      const Scalar& operator = (const E& r) {
      this->val_ = r.val();
      return *this;
    }

    T& val() { return val_; }
    const T& val() const { return val_; }
    operator T() const { return val_; }
  };

  template <class L, class Op, class R, typename T,
            class Ltt = void, class Rtt = void>
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
  };

  template <class L, class R, typename T>
  struct Expression<L, typename Ops<T>::Dot, R, T,
                    typename std::enable_if<isVector2<L>::value>::type,
                    typename std::enable_if<isVector2<R>::value>::type> {
    const L& l_;
    const R& r_;
    Expression(const L& l, const R& r) : l_(l), r_(r) {}

    T val() const {
      return Ops<T>::Add::apply(Ops<T>::Mul::apply(l_.x(), r_.x()),
                                Ops<T>::Mul::apply(l_.y(), r_.y()));
    }
  };

  template <class L, class Op, class R, typename T>
  struct Expression<L, Op, R, T,
                    typename std::enable_if<isScalar<L>::value>::type,
                    typename std::enable_if<isVector2<R>::value>::type> {
    const L& l_;
    const R& r_;
    Expression(const L& l, const R& r) : l_(l), r_(r) {}

    T x() const {
      return Op::apply(l_.val(), r_.x());
    }

    T y() const {
      return Op::apply(l_.val(), r_.y());
    }
  };

  template <class L, class Op, class R, typename T>
  struct Expression<L, Op, R, T,
                    typename std::enable_if<isVector2<L>::value>::type,
                    typename std::enable_if<isScalar<R>::value>::type> {
    const L& l_;
    const R& r_;
    Expression(const L& l, const R& r) : l_(l), r_(r) {}

    T x() const {
      return Op::apply(l_.x(), r_.val());
    }

    T y() const {
      return Op::apply(l_.y(), r_.val());
    }
  };

  template <class Op, class R, typename T>
  struct Expression<T, Op, R, T> {
    const T& l_;
    const R& r_;
    Expression(const T& l, const R& r) : l_(l), r_(r) {}

    T x() const {
      return Op::apply(l_, r_.x());
    }

    T y() const {
      return Op::apply(l_, r_.y());
    }
  };

  template <class L, class Op, typename T>
  struct Expression<L, Op, T, T> {
    const L& l_;
    const T& r_;
    Expression(const L& l, const T& r) : l_(l), r_(r) {}

    T x() const {
      return Op::apply(l_.x(), r_);
    }

    T y() const {
      return Op::apply(l_.y(), r_);
    }
  };

  template <typename T, class Vec3Enabled = void>
  struct Vector2;

  template <typename T>
  struct Vector2<T, typename std::enable_if<std::is_floating_point<T>::value>::type> final {
    T x_, y_;

    Vector2(void) : x_(0), y_(0) {}
    explicit Vector2(const T c) : x_(c), y_(c) {}
    explicit Vector2(const T (&a)[2]) : x_(a[0]), y_(a[1]) {}
    Vector2(const T x0, const T y0) : x_(x0), y_(y0) {}
    Vector2(const Vector2& src) : x_(src.x_), y_(src.y_) {}
    Vector2(const std::initializer_list<T> ilist) {
      assert(ilist.size() == 2);
      T* it = &x_;
      for (const auto i : ilist) *it++ = i;
    }
    template <class E>
      Vector2(const E& r) { *this = r; }

    template <class E>
      const Vector2& operator = (const E& r) {
      this->x_ = r.x();
      this->y_ = r.y();
      return *this;
    }

    T& x() { return x_; }
    const T& x() const { return x_; }
    T& y() { return y_; }
    const T& y() const { return y_; }

    const Vector2 operator - () const {
      return Vector2(-x_, -y_);
    }

    T& operator [] (const int i) {
      return (&x_)[i];
    }

    const T& operator [] (const int i) const {
      return (&x_)[i];
    }

    void clear(void) {
      x_ = y_ = 0.0;
    }

    T norm2(void) const {
      return x_*x_ + y_*y_;
    }

    T norm(void) const {
      return std::sqrt(norm2());
    }

    void normalize(void) {
      const auto r = norm();
      x_ /= r;
      y_ /= r;
    }

    const Vector2& operator += (const Vector2& rhs) {
      this->x_ += rhs.x_;
      this->y_ += rhs.y_;
      return *this;
    }

    const Vector2& operator -= (const Vector2& rhs) {
      this->x_ -= rhs.x_;
      this->y_ -= rhs.y_;
      return *this;
    }

    const Vector2& operator *= (const T c) {
      this->x_ *= c;
      this->y_ *= c;
      return *this;
    }

    const Vector2& operator /= (const T c) {
      this->x_ /= c;
      this->y_ /= c;
      return *this;
    }

    friend std::ostream& operator << (std::ostream& ost, const Vector2& v) {
      ost << "(" << v.x() << ", " << v.y() << ")";
      return ost;
    }

    template <class L, class R>
      friend Expression<L, typename Ops<T>::Add, R, T> operator + (const L& lhs, const R& rhs) {
      return Expression<L, typename Ops<T>::Add, R, T>(lhs, rhs);
    }

    template <class L, class R>
      friend Expression<L, typename Ops<T>::Sub, R, T> operator - (const L& lhs, const R& rhs) {
      return Expression<L, typename Ops<T>::Sub, R, T>(lhs, rhs);
    }

    template <class L, class R>
      friend Expression<L, typename Ops<T>::Mul, R, T> operator * (const L& lhs, const R& rhs) {
      return Expression<L, typename Ops<T>::Mul, R, T>(lhs, rhs);
    }

    template <class L, class R>
      friend Expression<L, typename Ops<T>::Div, R, T> operator / (const L& lhs, const R& rhs) {
      return Expression<L, typename Ops<T>::Div, R, T>(lhs, rhs);
    }

    // inner product
    template <class L, class R>
      friend Expression<L, typename Ops<T>::Dot, R, T> dot(const L& lhs, const R& rhs) {
      return Expression<L, typename Ops<T>::Dot, R, T>(lhs, rhs);
    }

    // cross product
    template <class L, class R>
      friend Expression<L, typename Ops<T>::Cross, R, T> operator ^ (const L& lhs, const R& rhs) {
      return Expression<L, typename Ops<T>::Cross, R, T>(lhs, rhs);
    }

    template <class E>
      friend Vector2 normalize(const E& rhs) {
      Vector2 eval = Vector2(rhs);
      eval.normalize();
      return eval;
    }
  };
}
#endif
