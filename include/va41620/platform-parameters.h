#pragma once

#include "metadata.h"
#include <cstdint>

namespace bootloader {

using namespace std;

/**
 * @brief Calculates, collects and gives access to all parameters for the platform that are known at
 * compile time.
 *
 */
class PlatformParameters {
private:
    static constexpr size_t SRAM_SIZE = 256 * 1024; // 256 KB

    static constexpr size_t NVM_TOTAL_SIZE = 2 * 1024 * 1024; // 2 MB
    static constexpr size_t BOOT_ROM_SIZE = 0x1000; // 4KB, see linkerscript
    static constexpr size_t APP_NVM_TOTAL_SIZE =
        NVM_TOTAL_SIZE - BOOT_ROM_SIZE; // Without BootRom, for Size see linker script
    // TODO Use Label for Begin Address? Not possible, because i wanna use constexpr
    // Use void* also not possible, because i want to do calculation
    static constexpr uintptr_t APP_NVM_BEGIN_ADDRESS = BOOT_ROM_SIZE; // 4KB

public:
    /**
     * @brief Number of images that should be saved in NVM.
     *
     */
    static constexpr size_t NUMBER_OF_IMAGES = 7;

    /**
     * @brief Max size of an image in NVM. It is limited by the size of the NVM divided by the
     * number of images and the size of Code SRAM without dedicated space for BOOT ROM Part.
     *
     */
    static constexpr size_t MAX_IMAGE_LENGTH =
        APP_NVM_TOTAL_SIZE / NUMBER_OF_IMAGES > SRAM_SIZE - BOOT_ROM_SIZE
            ? SRAM_SIZE - BOOT_ROM_SIZE
            : APP_NVM_TOTAL_SIZE / NUMBER_OF_IMAGES;

    /**
     * @brief Addresses where Image slots begin in NVM.
     *
     */
    static constexpr uintptr_t IMAGE_BEGIN_ADDRESSES[NUMBER_OF_IMAGES] = {
        APP_NVM_BEGIN_ADDRESS + 0 * MAX_IMAGE_LENGTH, APP_NVM_BEGIN_ADDRESS + 1 * MAX_IMAGE_LENGTH,
        APP_NVM_BEGIN_ADDRESS + 2 * MAX_IMAGE_LENGTH, APP_NVM_BEGIN_ADDRESS + 3 * MAX_IMAGE_LENGTH,
        APP_NVM_BEGIN_ADDRESS + 4 * MAX_IMAGE_LENGTH, APP_NVM_BEGIN_ADDRESS + 5 * MAX_IMAGE_LENGTH,
        APP_NVM_BEGIN_ADDRESS + 6 * MAX_IMAGE_LENGTH
    };

    /**
     * @brief Location of Register that protects writing to Code ROM.
     *
     */
    static constexpr uintptr_t ROM_PROT_ADDRESS = 0x40010010;

    // TODO Add Static Asserts
    static_assert(
        PlatformParameters::MAX_IMAGE_LENGTH + PlatformParameters::BOOT_ROM_SIZE <=
            PlatformParameters::SRAM_SIZE,
        "At least one Image + Bootloader needs to fit to Code SRAM");

    static_assert(
        sizeof(uintptr_t) == sizeof(void*),
        "uintptr_t and void* must be the same size. If that is not possible,  Metadata imageBegin "
        "needs to be initialized by hand and changed to void*!");

    static_assert(
        NUMBER_OF_IMAGES <= MAX_NUMBER_OF_IMAGES,
        "NUMBER_OF_IMAGES is bigger than MAX_NUMBER_OF_IMAGES");
};

} // namespace bootloader
