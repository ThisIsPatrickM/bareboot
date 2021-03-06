#pragma once

#include <cstdint>

extern "C" volatile uint32_t _estack[]; // NOLINT
extern "C" volatile uint32_t _sidata[]; // NOLINT
extern "C" volatile uint32_t _sdata[]; // NOLINT
extern "C" volatile uint32_t _edata[]; // NOLINT
extern "C" volatile uint32_t _sbss[]; // NOLINT
extern "C" volatile uint32_t _ebss[]; // NOLINT

// TODO Remove unised Labels
extern "C" uintptr_t __bootrom_start__; // NOLINT
extern "C" int __bootrom_size__; // NOLINT
extern "C" uintptr_t __approm_start__; // NOLINT
extern "C" int __approm_size__; // NOLINT
extern "C" uintptr_t __bootloader__; // NOLINT TODO Rename to __bootloader_start__?
