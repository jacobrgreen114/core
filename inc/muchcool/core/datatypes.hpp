
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#include <cstdint>
#include <memory>

#include "common.hpp"

namespace muchcool {

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

#if WORD_SIZE == 32
using uword = uint32;
using iword = int32;
#elif WORD_SIZE == 64
using uword = uint64;
using iword = int64;
#endif

#define null nullptr

}  // namespace muchcool