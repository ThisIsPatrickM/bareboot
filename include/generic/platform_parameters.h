#pragma once

#include "metadata.h"
#include <cstdint>

namespace bootloader {

using namespace std;

class PlatformParameters {
private:
public:
    static constexpr size_t NUMBER_OF_IMAGES = 4;
    static constexpr uint32_t MAX_IMAGE_LENGTH = 128;

    static_assert(
        sizeof(uintptr_t) == sizeof(void*),
        "uintptr_t and void* must be the same size. If that is not possible,  Metadata imageBegin "
        "needs to be initialized by hand and changed to void*!");

    static_assert(
        NUMBER_OF_IMAGES <= MAX_NUMBER_OF_IMAGES,
        "NUMBER_OF_IMAGES is bigger than MAX_NUMBER_OF_IMAGES");
};

} // namespace bootloader
