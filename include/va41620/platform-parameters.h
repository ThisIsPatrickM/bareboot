#pragma once

#include "../memory_map.h"
#include <cstdint>

namespace bootloader {

using namespace std;
/******************************************************************************
 *  Exposed Parameters
 * ***************************************************************************/
constexpr size_t NUMBER_OF_IMAGES = 7;

constexpr size_t SRAM_SIZE = 256 * 1024; // 256 KB
// TODO Check BootNVM Size!
constexpr size_t NVM_TOTAL_SIZE = 2 * 1024 * 1024; // 2 MB
constexpr size_t BOOT_ROM_SIZE = 0x1000; // 4KB, see linkerscript
constexpr size_t APP_NVM_TOTAL_SIZE =
    NVM_TOTAL_SIZE - BOOT_ROM_SIZE; // Without BootRom, for Size see linker script
constexpr uintptr_t APP_NVM_BEGIN_ADDRESS = BOOT_ROM_SIZE; // 4KB

constexpr size_t MAX_IMAGE_LENGTH =
    APP_NVM_TOTAL_SIZE / NUMBER_OF_IMAGES > SRAM_SIZE - BOOT_ROM_SIZE
        ? SRAM_SIZE - BOOT_ROM_SIZE
        : APP_NVM_TOTAL_SIZE / NUMBER_OF_IMAGES;

static_assert(
    MAX_IMAGE_LENGTH + BOOT_ROM_SIZE <= SRAM_SIZE,
    "At least one Image + Bootloader needs to fit to Code SRAM");

// TODO Double check and visualize and test this calculation
// TODO Check if uintptr_t is enough
constexpr uintptr_t IMAGE_BEGIN_ADDRESSES[NUMBER_OF_IMAGES] = {
    APP_NVM_BEGIN_ADDRESS + 0 * MAX_IMAGE_LENGTH, APP_NVM_BEGIN_ADDRESS + 1 * MAX_IMAGE_LENGTH,
    APP_NVM_BEGIN_ADDRESS + 2 * MAX_IMAGE_LENGTH, APP_NVM_BEGIN_ADDRESS + 3 * MAX_IMAGE_LENGTH,
    APP_NVM_BEGIN_ADDRESS + 4 * MAX_IMAGE_LENGTH, APP_NVM_BEGIN_ADDRESS + 5 * MAX_IMAGE_LENGTH,
    APP_NVM_BEGIN_ADDRESS + 6 * MAX_IMAGE_LENGTH,
};

} // namespace bootloader