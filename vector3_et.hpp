#if !defined VECTOR3_ET_HPP
#define VECTOR3_ET_HPP

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
  struct isVector3 final {
    DEFINE_MEMBER_EXISTENCE_FUNCS(x);
    DEFINE_MEMBER_EXISTENCE_FUNCS(y);
    DEFINE_MEMBER_EXISTENCE_FUNCS(z);
    static const bool value = exist_x::value && exist_y::value && exist_z::value;
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

    T z() const {
      return Op::apply(l_.z(), r_.z());
    }
  };

  template <class L, class R, typename T>
  struct Expression<L, typename Ops<T>::Cross, R, T,
                    typename std::enable_if<isVector3<L>::value>::type,
                    typename std::enable_if<isVector3<R>::value>::type> {
    const L& l_;
    const R& r_;
    Expression(const L& l, const R& r) : l_(l), r_(r) {}

    T x() const {
      return Ops<T>::Sub::apply(Ops<T>::Mul::apply(l_.y(), r_.z()),
                                Ops<T>::Mul::apply(l_.z(), r_.y()));
    }

    T y() const {
      return Ops<T>::Sub::apply(Ops<T>::Mul::apply(l_.z(), r_.x()),
                                Ops<T>::Mul::apply(l_.x(), r_.z()));
    }

    T z() const {
      return Ops<T>::Sub::apply(Ops<T>::Mul::apply(l_.x(), r_.y()),
                                Ops<T>::Mul::apply(l_.y(), r_.x()));
    }
  };

  template <class L, class R, typename T>
  struct Expression<L, typename Ops<T>::Dot, R, T,
                    typename std::enable_if<isVector3<L>::value>::type,
                    typename std::enable_if<isVector3<R>::value>::type> {
    const L& l_;
    const R& r_;
    Expression(const L& l, const R& r) : l_(l), r_(r) {}

    T val() const {
      return Ops<T>::Add::apply(Ops<T>::Add::apply(Ops<T>::Mul::apply(l_.x(), r_.x()),
                                                   Ops<T>::Mul::apply(l_.y(), r_.y())),
                                Ops<T>::Mul::apply(l_.z(), r_.z()));
    }
  };

  template <class L, class Op, class R, typename T>
  struct Expression<L, Op, R, T,
                    typename std::enable_if<isScalar<L>::value>::type,
                    typename std::enable_if<isVector3<R>::value>::type> {
    const L& l_;
    const R& r_;
    Expression(const L& l, const R& r) : l_(l), r_(r) {}

    T x() const {
      return Op::apply(l_.val(), r_.x());
    }

    T y() const {
      return Op::apply(l_.val(), r_.y());
    }

    T z() const {
      return Op::apply(l_.val(), r_.z());
    }
  };

  template <class L, class Op, class R, typename T>
  struct Expression<L, Op, R, T,
                    typename std::enable_if<isVector3<L>::value>::type,
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

    T z() const {
      return Op::apply(l_.z(), r_.val());
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

    T z() const {
      return Op::apply(l_, r_.z());
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

    T z() const {
      return Op::apply(l_.z(), r_);
    }
  };

  template <typename T, class Vec3Enabled = void>
  struct Vector3;

  template <typename T>
  struct Vector3<T, typename std::enable_if<std::is_floating_point<T>::value>::type> final {
    T x_, y_, z_;

    Vector3(void) : x_(0), y_(0), z_(0) {}
    explicit Vector3(const T c) : x_(c), y_(c), z_(c) {}
    explicit Vector3(const T (&a)[3]) : x_(a[0]), y_(a[1]), z_(a[2]) {}
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

#define DEFINE_OP_FUNCS0(OP, OP_NAME)                                   \
    friend Expression<Vector3,                                          \
                      typename OP_NAME,                                 \
                      Vector3,                                          \
                      T>                                                \
      operator OP (const Vector3& lhs, const Vector3& rhs) {            \
      return Expression<Vector3, typename OP_NAME, Vector3, T>(lhs, rhs); \
    }                                                                   \
                                                                        \
    template <class LL, class OPL, class RL, class LR, class OPR, class RR> \
      friend Expression<Expression<LL, OPL, RL, T>,                     \
                        typename OP_NAME,                               \
                        Expression<LR, OPR, RR, T>,                     \
                        T>                                              \
      operator OP (const Expression<LL, OPL, RL, T>& lhs,               \
                   const Expression<LR, OPR, RR, T>& rhs) {             \
      return Expression<Expression<LL, OPL, RL, T>, typename OP_NAME, Expression<LR, OPR, RR, T>, T>(lhs, rhs); \
    }
    DEFINE_OP_FUNCS0(+, Ops<T>::Add)
    DEFINE_OP_FUNCS0(-, Ops<T>::Sub)
    DEFINE_OP_FUNCS0(^, Ops<T>::Cross)
#undef DEFINE_OP_FUNCS0

    friend Expression<Vector3,
                      typename Ops<T>::Dot,
                      Vector3,
                      T>
    operator * (const Vector3& lhs, const Vector3& rhs) {
      return Expression<Vector3, typename Ops<T>::Dot, Vector3, T>(lhs, rhs);
    }

#define DEFINE_OP_FUNCS1(OP, OP_NAME)                                   \
    friend Expression<T,                                                \
                      typename OP_NAME,                                 \
                      Vector3,                                          \
                      T>                                                \
      operator OP (const T& lhs, const Vector3& rhs) {                  \
      return Expression<T, typename OP_NAME, Vector3, T>(lhs, rhs);     \
    }                                                                   \
                                                                        \
    friend Expression<Vector3,                                          \
                      typename OP_NAME,                                 \
                      T,                                                \
                      T>                                                \
      operator OP (const Vector3& lhs, const T& rhs) {                  \
      return Expression<Vector3, typename OP_NAME, T, T>(lhs, rhs);     \
    }                                                                   \
                                                                        \
    friend Expression<Scalar<T>,                                        \
                      typename OP_NAME,                                 \
                      Vector3,                                          \
                      T>                                                \
      operator OP (const Scalar<T>& lhs, const Vector3& rhs) {          \
      return Expression<Scalar<T>, typename OP_NAME, Vector3, T>(lhs, rhs); \
    }                                                                   \
                                                                        \
    friend Expression<Vector3,                                          \
                      typename OP_NAME,                                 \
                      Scalar<T>,                                        \
                      T>                                                \
      operator OP (const Vector3& lhs, const Scalar<T>& rhs) {          \
      return Expression<Vector3, typename OP_NAME, Scalar<T>, T>(lhs, rhs); \
    }                                                                   \
                                                                        \
    template <class LL, class OPL, class RL, class LR, class OPR, class RR> \
      friend Expression<Expression<LL, OPL, RL, T>,                     \
                        typename OP_NAME,                               \
                        Expression<LR, OPR, RR, T>,                     \
                        T>                                              \
      operator OP (const Expression<LL, OPL, RL, T>& lhs,               \
                   const Expression<LR, OPR, RR, T>& rhs) {             \
      return Expression<Expression<LL, OPL, RL, T>, typename OP_NAME, Expression<LR, OPR, RR, T>, T>(lhs, rhs); \
    }
    DEFINE_OP_FUNCS1(*, Ops<T>::Mul)
    DEFINE_OP_FUNCS1(/, Ops<T>::Div)
#undef DEFINE_OP_FUNCS1

    template <class E>
      friend Vector3 normalize(const E& rhs) {
      Vector3 eval = Vector3(rhs);
      eval.normalize();
      return eval;
    }
  };
}
#endif
