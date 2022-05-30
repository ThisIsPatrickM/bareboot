#pragma once

#include <cstdint>

extern "C" volatile uint32_t _estack[]; // NOLINT
extern "C" volatile uint32_t _sidata[]; // NOLINT
extern "C" volatile uint32_t _sdata[]; // NOLINT
extern "C" volatile uint32_t _edata[]; // NOLINT
extern "C" volatile uint32_t _sbss[]; // NOLINT
extern "C" volatile uint32_t _ebss[]; // NOLINT

// tODO Maybe int is not the best choice
extern void* __bootrom_start__; // NOLINT
extern int __bootrom_size__; // NOLINT
extern void* __approm_start__; // NOLINT
extern int __approm_size__; // NOLINT
extern void* __bootloader__; // NOLINT
