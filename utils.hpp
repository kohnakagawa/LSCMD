#if !defined UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <memory>
#include <type_traits>

#define LOCAL_STRESS_CONCATENATE(x, y) x ## y
#define LOCAL_STRESS_CONCAT(x, y) LOCAL_STRESS_CONCATENATE(x, y)

#define ERR(msg)                                                        \
  do {                                                                  \
    std::cerr << "Error occurs at " << __FILE__ << ":" << __LINE__ << std::endl; \
    std::cerr << msg << std::endl;                                      \
    std::exit(1);                                                       \
  } while (0)

namespace LocalStress {
  template<typename T, typename... Args>
  std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }

  template <class T>
  using remove_reference_t = typename std::remove_reference<T>::type;

  template <class T>
  using remove_const_t = typename std::remove_const<T>::type;

  bool is_little_endian() {
    union {
      int16_t s;
      int8_t  c[2];
    } tmp;
    tmp.s = 0x1;
    return (tmp.c[0] == 0x1);
  }
}
#endif
