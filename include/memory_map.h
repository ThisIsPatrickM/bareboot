#pragma once

#include <cstdint>

extern "C" volatile uint32_t _estack[]; // NOLINT
extern "C" volatile uint32_t _sidata[]; // NOLINT
extern "C" volatile uint32_t _sdata[]; // NOLINT
extern "C" volatile uint32_t _edata[]; // NOLINT
extern "C" volatile uint32_t _sbss[]; // NOLINT
extern "C" volatile uint32_t _ebss[]; // NOLINT

// TODO Maybe int is not the best choice, or not all of them are needed!
extern "C" void* __bootrom_start__; // NOLINT
extern "C" int __bootrom_size__; // NOLINT
extern "C" void* __approm_start__; // NOLINT
extern "C" int __approm_size__; // NOLINT
extern "C" void* __bootloader__; // NOLINT
