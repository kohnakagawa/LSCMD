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
}
#endif
