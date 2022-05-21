#pragma once
#include "peripheral_ctrl/aborting_error.h"
#include "rodos_includes/hal_gpio.h"
#include <cstdint>

namespace RODOS::pin_config {

enum class OUTPUT_TYPE : uint8_t { PUSHPULL, OPENDRAIN };

enum class PIN_FUNCTION : uint8_t { FUN0, FUN1, FUN2, FUN3 };

enum class PULL : uint8_t { OFF, UP, DOWN };

constexpr auto TOTAL_PINS = 104;

class VA41620_PIN {
public:
    constexpr VA41620_PIN(GPIO_PIN pin)
        : m_idx { static_cast<int8_t>(pin) }
    {
        if (pin >= TOTAL_PINS || pin < -1) {
            // this will only happen while constructors are running, so there'd be no use in
            // printing a message
            __builtin_trap();
        }
    }

    constexpr VA41620_PIN(int8_t pin)
        : m_idx { pin }
    {
        if (pin >= TOTAL_PINS || pin < -1) {
            // this will only happen while constructors are running, so there'd be no use in
            // printing a message
            __builtin_trap();
        }
    }

    constexpr VA41620_PIN()
        : m_idx { -1 }
    {
    }

    constexpr bool isValid() { return m_idx > -1; }

    constexpr operator int8_t() const { return m_idx; }

    constexpr int8_t getPinInPort() const { return m_idx % 16; }

    constexpr int8_t getPortNum() const { return m_idx / 16; }

private:
    int8_t m_idx;
};

enum class SPI_NSS_INDEX : int8_t { SS0 = 0, SS1, SS2, SS3, SS4, SS5, SS6, SS7, INVALID = -1 };

}
