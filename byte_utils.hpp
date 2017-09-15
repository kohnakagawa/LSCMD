#if !defined BYTE_MANIP_HPP
#define BYTE_MANIP_HPP

#include <fstream>

namespace LocalStress {
  static inline bool is_lsb_first(void) {
    const int16_t s = 0x1234;
    return (*(int8_t*)&s == 0x34);
  }

  static inline bool is_msb_first(void) {
    const int16_t s = 0x1234;
    return (*(int8_t*)&s == 0x12);
  }

  static inline uint32_t byte_swap(const int in) {
    return byte_swap((uint32_t)in);
  }

  static inline uint32_t byte_swap(const uint32_t in) {
    return
      ((in & 0x000000ff) << 24) |
      ((in & 0x0000ff00) <<  8) |
      ((in & 0x00ff0000) >>  8) |
      ((in & 0xff000000) >> 24);
  }

  static inline uint64_t byte_swap(const uint64_t in) {
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

  static inline uint32_t conv2uint(const uint32_t in) {
    return in;
  }

  static inline uint32_t conv2uint(const int in) {
    return in;
  }

  static inline uint32_t conv2uint(const float in) {
    union {
      float f;
      uint32_t ui;
    } tmp;
    tmp.f = in;
    return tmp.ui;
  }

  static inline uint64_t conv2uint(const double in) {
    union {
      double d;
      uint64_t ui;
    } tmp;
    tmp.d = in;
    return tmp.ui;
  }

  template <typename S>
  auto conv2_lsb_first_if_needed(const S in) -> decltype(conv2uint(in)) {
    if (is_lsb_first()) {
      return conv2uint(in);
    } else if (is_msb_first()) {
      return byte_swap(conv2uint(in));
    } else {
      LOCAL_STRESS_ERR("Non-supported endian.");
      return -1;
    }
  }

  template <typename T>
  static inline void write_as_lsbfirst(std::ofstream& fout,
                                       const T in) {
    const auto dat = conv2_lsb_first_if_needed(in);
    fout.write(reinterpret_cast<const char*>(&dat), sizeof(T));
  }

  template <>
  void write_as_lsbfirst<std::string>(std::ofstream& fout, const std::string in) {
    fout.write(in.c_str(), in.length() * sizeof(char));
  }
}
#endif
