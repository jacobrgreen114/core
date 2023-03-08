
#pragma once

#include <cstdint>
#include <memory>

#include "Platform.hpp"

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

#if WORD_SIZE == 32
using uintn = uint32;
using intn = int32;
#elif WORD_SIZE == 64
using uintn = uint64;
using intn = int64;
#endif

#define null nullptr
