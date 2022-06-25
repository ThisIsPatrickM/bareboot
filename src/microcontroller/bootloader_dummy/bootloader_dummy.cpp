#include "generic/platform-parameters.h"
#include "metadata.h"

using namespace bootloader;

/* Dummy Declarations */
uint8_t imageBuffer[PlatformParameters::MAX_IMAGE_LENGTH * PlatformParameters::NUMBER_OF_IMAGES] = {
    0
};

// Initialize Dummy, Assuming there are 4 Images
GlobalImageMetadata dummyBootloader {};

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

uintptr_t __bootloader__ = reinterpret_cast<uintptr_t>(&dummyBootloader); // NOLINT
