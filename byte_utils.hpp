#if !defined BYTE_MANIP_HPP
#define BYTE_MANIP_HPP

namespace LocalStress {
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
