#include "memcpy/memcpy.h"

namespace bootloader::memcpy {

void* Memcpy(void* dest, const void* src, size_t n)
{
    if (dest == nullptr || src == nullptr) {
        return nullptr;
    }

    uint32_t iterations = n / sizeof(uint32_t);
    uint32_t remain = n % sizeof(uint32_t);

    auto* destP = static_cast<uint32_t*>(dest);
    const auto* srcP = static_cast<const uint32_t*>(src);

    for (uint32_t i = 0; i < iterations; i++) {
        *destP = *srcP;
        destP++; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        srcP++; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }

    // Adjust Remain
    if (remain > 0) {
        // Reset bytes that will be overwritten
        *destP = *destP & (0xFFFFFFFF >> remain * 8);
        // Set Byte that will be overwritten
        *destP = *srcP | (0xFFFFFFFF << remain * 8);
    }

    return dest;
}

}
