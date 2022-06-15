#include "generic/platform-parameters.h"
#include "metadata.h"

/* Dummy Declarations */
uint8_t imageBuffer
    [bootloader::PlatformParameters::MAX_IMAGE_LENGTH *
     bootloader::PlatformParameters::NUMBER_OF_IMAGES] = { 0 };

// Initialize Dummy, Assuming there are 4 Images
bootloader::GlobalImageMetadata dummyBootloader {};

uintptr_t __approm_start__ = reinterpret_cast<uintptr_t>(imageBuffer); // NOLINT

// NOLINTNEXTLINE(readability-identifier-naming)
int __approm_size__ = bootloader::PlatformParameters::MAX_IMAGE_LENGTH *
                      bootloader::PlatformParameters::NUMBER_OF_IMAGES;
uintptr_t __bootloader__ = reinterpret_cast<uintptr_t>(&dummyBootloader); // NOLINT
