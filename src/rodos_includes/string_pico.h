#pragma once

#include <cstddef>
#include <cstdint>

namespace rodos {

extern "C" void* memcpy(void* dest, const void* src, size_t len) __attribute__((used)); // NOLINT

} // namespace
