#pragma once

#include <cstdint>

namespace bootloader {

using namespace std;

class PlatformParameters {
private:
    static constexpr size_t BOOT_ROM_SIZE = 0x1000; // 4KB, see linkerscript
public:
    static constexpr size_t NUMBER_OF_IMAGES = 3;
    // TODO Dont know what MAX_IMAGE Length makes sense for generic
    static constexpr uint32_t MAX_IMAGE_LENGTH = 128;
    // TODO How to declare IMAGE_BEGIN_ADDRESSES? For Dummy implementation and generic?
    // Maybe i need to seperate?
    // static constexpr uintptr_t IMAGE_BEGIN_ADDRESSES[NUMBER_OF_IMAGES] = {
    //     0x00000000,
    //     0x00001000,
    //     0x00002000,
    // };
};

} // namespace bootloader
