#pragma once
#include <stdint.h>

namespace RODOS {

/**
 * Represents a single memory mapped IO register
 * Functions are using inline assembly because it wouldn't be possible to write with 32 bit and 16
 * bit instructions to the same memory location without breaking C++ strict aliasing rules otherwise
 */
class MMIO {
private:
    volatile uint32_t reg;

public:
    using value_t = uint32_t;
    inline void write(const uint32_t value)
    {
        asm volatile("str %1, %0" : "=m"(reg) : "r"((uint32_t)value));
    }

    inline void write8(const uint8_t value)
    {
        asm volatile("strb %1, %0" : "=m"(reg) : "r"((uint32_t)value));
    }

    inline void write16(const uint16_t value)
    {
        asm volatile("strh %1, %0" : "=m"(reg) : "r"((uint32_t)value));
    }

    inline uint32_t read() const
    {
        uint32_t result;
        asm volatile("ldr %0, %1" : "=r"(result) : "m"(reg));
        return result;
    }

    inline uint8_t read8() const
    {
        uint32_t result;
        asm volatile("ldrb %0, %1" : "=r"(result) : "m"(reg));
        return ((uint8_t)result);
    }

    inline uint16_t read16() const
    {
        uint32_t result;
        asm volatile("ldrh %0, %1" : "=r"(result) : "m"(reg));
        return ((uint16_t)result);
    }
};

class MMIO8 {
private:
    volatile uint8_t reg;

public:
    using value_t = uint8_t;
    inline void write(const value_t value) { asm volatile("strb %1, %0" : "=m"(reg) : "r"(value)); }

    inline value_t read() const
    {
        uint32_t result;
        asm volatile("ldrb %0, %1" : "=r"(result) : "m"(reg));
        return static_cast<value_t>(result);
    }
};
}
