#if !defined UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <memory>
#include <type_traits>

#define LOCAL_STRESS_CONCATENATE(x, y) x ## y
#define LOCAL_STRESS_CONCAT(x, y) LOCAL_STRESS_CONCATENATE(x, y)

#define LOCAL_STRESS_UNUSED_VAR(x) (void) x

#define LOCAL_STRESS_ERR(msg)                                                        \
  do {                                                                  \
    std::cerr << "Error occurs at " << __FILE__ << ":" << __LINE__ << std::endl; \
    std::cerr << msg << std::endl;                                      \
    std::exit(1);                                                       \
  } while (0)

namespace LocalStress {
  template <class T>
  using remove_reference_t = typename std::remove_reference<T>::type;

  template <class T>
  using remove_const_t = typename std::remove_const<T>::type;

  template <class T>
  using remove_extent_t = typename std::remove_extent<T>::type;

  template <class T, class ...Args>
  typename std::enable_if<!std::is_array<T>::value, std::unique_ptr<T>>::type
  make_unique(Args&& ...args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }

  template <class T>
  typename std::enable_if<std::is_array<T>::value, std::unique_ptr<T>>::type
  make_unique(std::size_t N) {
    return std::unique_ptr<T>(new remove_extent_t<T> [N]);
  }

  bool is_lsb_first(void) {
    const int16_t s = 0x1234;
    return (*(int8_t*)&s == 0x34);
  }

  bool is_msb_first(void) {
    const int16_t s = 0x1234;
    return (*(int8_t*)&s == 0x12);
  }

  uint32_t byte_swap(const int in) {
    return byte_swap(in);
  }

  uint32_t byte_swap(const uint32_t in) {
    return
      ((in & 0x000000ff) << 24) |
      ((in & 0x0000ff00) <<  8) |
      ((in & 0x00ff0000) >>  8) |
      ((in & 0xff000000) >> 24);
  }

  uint64_t byte_swap(const uint64_t in) {
    return
      ((in & 0x00000000000000ff) << 56) |
      ((in & 0x000000000000ff00) << 40) |
      ((in & 0x0000000000ff0000) << 24) |
      ((in & 0x00000000ff000000) <<  8) |
      ((in & 0x000000ff00000000) >>  8) |
      ((in & 0x0000ff0000000000) >> 24) |
      ((in & 0x00ff000000000000) >> 40) |
      ((in & 0xff00000000000000) >> 56);
  }

  uint32_t conv2uint(const uint32_t in) {
    return in;
  }

  uint32_t conv2uint(const int in) {
    return in;
  }

  uint32_t conv2uint(const float in) {
    union {
      float f;
      uint32_t ui;
    } tmp;
    tmp.f = in;
    return tmp.ui;
  }

  uint64_t conv2uint(const double in) {
    union {
      double d;
      uint64_t ui;
    } tmp;
    tmp.d = in;
    return tmp.ui;
  }
}
#endif
