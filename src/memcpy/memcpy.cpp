#include "memcpy/memcpy.h"

namespace bootloader::memcpy {

void* va41620UnsignedMemcpy(void* __restrict__ dest, const void* __restrict__ src, std::size_t n)

{
    if (dest == nullptr || src == nullptr) {
        return nullptr;
    }

    unsigned iterations = n / sizeof(unsigned);
    int remain = static_cast<int>(n % sizeof(unsigned));

    auto* destP = static_cast<unsigned*>(dest);
    const auto* srcP = static_cast<const unsigned*>(src);

    for (unsigned i = 0; i < iterations; i++) {
        destP[i] = srcP[i]; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }

    // Adjust Remain
    if (remain > 0) {
        unsigned mask = [&]() {
            if constexpr (std::endian::native == std::endian::little) {
                return (~0x0U) << (remain * 8);
            } else {
                return (~0x0U) >> (remain * 8);
            }
        }();
        // Reset bytes that will be overwritten
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        destP[iterations] = destP[iterations] & mask;
        // Set bytes that will be overwritten
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        destP[iterations] |= srcP[iterations] & ~mask;
    }

    return dest;
}

}

namespace RODOS {

// NOLINTBEGIN
void* memcpy(void* destP, const void* sP, std::size_t len)
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
