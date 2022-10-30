#pragma once

#include <cstdint>

namespace RODOS {

/**
 * Represents a single memory mapped IO m_register
 * Functions are using inline assembly because it wouldn't be possible to write with 32 bit and 16
 * bit instructions to the same memory location without breaking C++ strict aliasing rules otherwise
 */
class MMIO {
private:
    volatile uint32_t m_reg;

public:
    using value_t = uint32_t; // NOLINT
    inline void write(const uint32_t value)
    {
        asm volatile("str %1, %0" : "=m"(m_reg) : "r"((uint32_t)value));
    }

    inline void write8(const uint8_t value)
    {
        asm volatile("strb %1, %0" : "=m"(m_reg) : "r"((uint32_t)value));
    }

    inline void write16(const uint16_t value)
    {
        asm volatile("strh %1, %0" : "=m"(m_reg) : "r"((uint32_t)value));
    }

    [[nodiscard]] inline uint32_t read() const
    {
        uint32_t result = 0;
        asm volatile("ldr %0, %1" : "=r"(result) : "m"(m_reg));
        return result;
    }

    [[nodiscard]] inline uint8_t read8() const
    {
        uint32_t result = 0;
        asm volatile("ldrb %0, %1" : "=r"(result) : "m"(m_reg));
        return ((uint8_t)result);
    }

    [[nodiscard]] inline uint16_t read16() const
    {
        uint32_t result = 0;
        asm volatile("ldrh %0, %1" : "=r"(result) : "m"(m_reg));
        return ((uint16_t)result);
    }
};

class MMIO8 {
private:
    volatile uint8_t m_reg;

public:
    using value_t = uint8_t; // NOLINT
    inline void write(const value_t value)
    {
        asm volatile("strb %1, %0" : "=m"(m_reg) : "r"(value));
    }

    [[nodiscard]] inline value_t read() const
    {
        uint32_t result = 0;
        asm volatile("ldrb %0, %1" : "=r"(result) : "m"(m_reg));
        return static_cast<value_t>(result);
    }
};
}
