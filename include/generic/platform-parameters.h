#pragma once

#include <cstdint>

namespace bootloader {

class PlatformParameters {
public:
    static constexpr std::size_t NUMBER_OF_IMAGES = 3;
    static constexpr uint32_t MAX_IMAGE_LENGTH =
        0xFFFF; // TODO: Dunno what is a good value for generic
};

} // namespace bootloader
