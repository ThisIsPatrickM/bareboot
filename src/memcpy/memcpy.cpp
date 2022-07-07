#include "memcpy/memcpy.h"

namespace bootloader::memcpy {

void* memcpy(void* __restrict__ dest, const void* __restrict__ src, size_t n)

{
    if (dest == nullptr || src == nullptr) {
        return nullptr;
    }

    unsigned iterations = n / sizeof(unsigned);
    int remain = static_cast<int>(n % sizeof(unsigned));

    auto* destP = static_cast<unsigned*>(dest);
    const auto* srcP = static_cast<const unsigned*>(src);

    for (unsigned i = 0; i < iterations; i++) {
        *destP = *srcP;
        destP++; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        srcP++; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }

    // Adjust Remain
    if (remain > 0) {
        // Reset bytes that will be overwritten
        *destP = *destP & (~unsigned(0x0) << remain * 8);
        // Set bytes that will be overwritten
        *destP |= *srcP & (~unsigned(0x0) >> (sizeof(unsigned) - remain) * 8);
    }

    return dest;
}

}

namespace rodos {

// NOLINTBEGIN
void* memcpy(void* destP, const void* sP, size_t len)
{
    uint8_t* dest = (uint8_t*)destP;
    const uint8_t* s = (const uint8_t*)sP;
    if (!dest || !s)
        return 0;
    while (len) {
        // dest[len] = s[len];
        *dest = *s;
        len--;
        s++;
        dest++;
    }
    return destP;
}
// NOLINTEND
}