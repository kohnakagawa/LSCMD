#include "gtest/gtest.h"
#include <cstdint>
#include "../byte_utils.hpp"

using namespace LocalStress;

TEST(byte_utils, byte_swap32) {
  const uint32_t a = 0x12345678;
  ASSERT_EQ(byte_swap(a), 0x78563412);
}

TEST(byte_utils, byte_swap64) {
  const uint64_t a = 0x1234567887654321;
  ASSERT_EQ(byte_swap(a), 0x2143658778563412);
}
