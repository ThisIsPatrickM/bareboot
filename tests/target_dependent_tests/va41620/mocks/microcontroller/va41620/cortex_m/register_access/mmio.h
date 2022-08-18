#pragma once

#include <cstdint>

namespace RODOS {

class MMIO {
private:
    volatile uint32_t m_reg;

public:
    using value_t = uint32_t; // NOLINT
    inline void write([[maybe_unused]] const uint32_t value) {}

    inline void write8([[maybe_unused]] const uint8_t value) {}

    inline void write16([[maybe_unused]] const uint16_t value) {}

    [[nodiscard]] inline uint32_t read() const { return 0; }

    [[nodiscard]] inline uint8_t read8() const { return 0; }

    [[nodiscard]] inline uint16_t read16() const { return 0; }
};

class MMIO8 {
private:
    volatile uint8_t m_reg;

public:
    using value_t = uint8_t; // NOLINT
    inline void write([[maybe_unused]] const value_t value) {}

    [[nodiscard]] inline value_t read() const { return 0; }
};
}
