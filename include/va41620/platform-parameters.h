#pragma once

#include "../memory_map.h"
#include <cstdint>

namespace bootloader {

using namespace std;
class PlatformParameters {
private:
    static constexpr size_t SRAM_SIZE = 256 * 1024; // 256 KB

    static constexpr size_t NVM_TOTAL_SIZE = 2 * 1024 * 1024; // 2 MB
    static constexpr size_t BOOT_ROM_SIZE = 0x1000; // 4KB, see linkerscript
    static constexpr size_t APP_NVM_TOTAL_SIZE =
        NVM_TOTAL_SIZE - BOOT_ROM_SIZE; // Without BootRom, for Size see linker script
    // TODO Use Label for Begin Address? Not possible, because i wanna use constexpr
    static constexpr uintptr_t APP_NVM_BEGIN_ADDRESS = BOOT_ROM_SIZE; // 4KB
public:
    static constexpr size_t NUMBER_OF_IMAGES = 7;
    static constexpr size_t MAX_IMAGE_LENGTH =
        APP_NVM_TOTAL_SIZE / NUMBER_OF_IMAGES > SRAM_SIZE - BOOT_ROM_SIZE
            ? SRAM_SIZE - BOOT_ROM_SIZE
            : APP_NVM_TOTAL_SIZE / NUMBER_OF_IMAGES;
    static constexpr uintptr_t IMAGE_BEGIN_ADDRESSES[NUMBER_OF_IMAGES] = {
        APP_NVM_BEGIN_ADDRESS + 0 * MAX_IMAGE_LENGTH, APP_NVM_BEGIN_ADDRESS + 1 * MAX_IMAGE_LENGTH,
        APP_NVM_BEGIN_ADDRESS + 2 * MAX_IMAGE_LENGTH, APP_NVM_BEGIN_ADDRESS + 3 * MAX_IMAGE_LENGTH,
        APP_NVM_BEGIN_ADDRESS + 4 * MAX_IMAGE_LENGTH, APP_NVM_BEGIN_ADDRESS + 5 * MAX_IMAGE_LENGTH,
        APP_NVM_BEGIN_ADDRESS + 6 * MAX_IMAGE_LENGTH,
    };

    // TODO Add Static Asserts
    // TODO Document this class properly
    static_assert(
        PlatformParameters::MAX_IMAGE_LENGTH + PlatformParameters::BOOT_ROM_SIZE <=
            PlatformParameters::SRAM_SIZE,
        "At least one Image + Bootloader needs to fit to Code SRAM");
};

} // namespace bootloader
