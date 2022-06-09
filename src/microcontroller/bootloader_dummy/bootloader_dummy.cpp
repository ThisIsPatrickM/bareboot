#include "generic/platform-parameters.h"
#include "metadata.h"

#include <iostream>
#include <sys/mman.h>

/* Dummy Implementations */

uint8_t imageBuffer
    [bootloader::PlatformParameters::MAX_IMAGE_LENGTH *
     bootloader::PlatformParameters::NUMBER_OF_IMAGES] = { 0 };

// Initialize Dummy, Assuming there are 3 Images
bootloader::GlobalImageMetadata dummyBootloader {};

uintptr_t __approm_start__ = (uintptr_t)imageBuffer; // NOLINT
int __approm_size__ = bootloader::PlatformParameters::MAX_IMAGE_LENGTH * // NOLINT
                      bootloader::PlatformParameters::NUMBER_OF_IMAGES;
uintptr_t __bootloader__ = (uintptr_t)&dummyBootloader; // NOLINT
