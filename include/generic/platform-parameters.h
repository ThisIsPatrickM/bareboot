#pragma once

#include "metadata.h"
#include <cstdint>

namespace bootloader {

using namespace std;

class PlatformParameters {
private:
public:
    static constexpr size_t NUMBER_OF_IMAGES = 4;
    // TODO Dont know what MAX_IMAGE Length makes sense for generic
    static constexpr uint32_t MAX_IMAGE_LENGTH = 128;
    // TODO How to declare IMAGE_BEGIN_ADDRESSES? For Dummy implementation and generic?
    // Maybe i need to seperate?
    // static constexpr uintptr_t IMAGE_BEGIN_ADDRESSES[NUMBER_OF_IMAGES] = {
    //     0x00000000,
    //     0x00001000,
    //     0x00002000,
    // };

    static_assert(
        sizeof(uintptr_t) == sizeof(void*),
        "uintptr_t and void* must be the same size. If that is not possible,  Metadata imageBegin "
        "needs to be initialized by hand and changed to void*!");

    static_assert(
        NUMBER_OF_IMAGES <= MAX_NUMBER_OF_IMAGES,
        "NUMBER_OF_IMAGES is bigger than MAX_NUMBER_OF_IMAGES");
};

} // namespace bootloader
