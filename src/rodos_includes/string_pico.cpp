#include "string_pico.h"

namespace rodos {

// NOLINTBEGIN
extern "C" void* memcpy(void* destP, const void* sP, size_t len)
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
