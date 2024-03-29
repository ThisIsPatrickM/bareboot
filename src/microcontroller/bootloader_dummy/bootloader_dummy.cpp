#include "metadata.h"
#include "platform_parameters.h"

using namespace bootloader;

/* Dummy Declarations */
uint8_t imageBuffer[PlatformParameters::MAX_IMAGE_LENGTH * PlatformParameters::NUMBER_OF_IMAGES] = {
    0
};

// Initialize Dummy, Assuming there are 4 Images
GlobalImageMetadata dummyMetadata {
    .images = { { .imageBegin = reinterpret_cast<uintptr_t>( // NOLINT
                      &imageBuffer[PlatformParameters::MAX_IMAGE_LENGTH * 0]) },
                { .imageBegin = reinterpret_cast<uintptr_t>( // NOLINT
                      &imageBuffer[PlatformParameters::MAX_IMAGE_LENGTH * 1]) },
                { .imageBegin = reinterpret_cast<uintptr_t>( // NOLINT
                      &imageBuffer[PlatformParameters::MAX_IMAGE_LENGTH * 2]) },
                { .imageBegin = reinterpret_cast<uintptr_t>( // NOLINT
                      &imageBuffer[PlatformParameters::MAX_IMAGE_LENGTH * 3]) } }
};

uintptr_t __approm_start__ = reinterpret_cast<uintptr_t>(imageBuffer); // NOLINT

uintptr_t __bootloader__ = reinterpret_cast<uintptr_t>(&dummyMetadata); // NOLINT

uint32_t _sidata[1]; // NOLINT(readability-identifier-naming)
uint32_t _sdata[1]; // NOLINT(readability-identifier-naming)
uint32_t _sbss[1]; // NOLINT(readability-identifier-naming)
uint32_t _ebss[1]; // NOLINT(readability-identifier-naming)
