#pragma once
#include "peripheral_ctrl/pin_config/enums.h"
#include "peripheral_ctrl/register_maps/gpio_registers.h"
#include "rodos_includes/hal_gpio.h"

namespace RODOS {

namespace GPIO {
    enum class MODE : uint8_t {
        INPUT = 0,
        OUTPUT,
    };
} // namespace GPIO

class HW_HAL_GPIO {
public:
    HW_HAL_GPIO(pin_config::VA41620_PIN pinIdx, HAL_GPIO& owner);

    int32_t init(bool isOutput, uint8_t numOfPins, uint32_t initVal);
    int32_t setMode(GPIO::MODE mode);
    int32_t setNumPins(uint32_t numPins);
    int32_t setPull(pin_config::PULL pull);

    void reset();
    void setOutput(uint16_t val);
    uint16_t readInput();

    void enableInterrupts();
    void disableInterrupts();

    bool m_interruptEventOccurred;
    GPIO_IRQ_SENSITIVITY m_irqSensitivity;

private:
    pin_config::VA41620_PIN m_idx;
    HAL_GPIO& m_owner;
    int8_t m_portNum;
    uint16_t m_pinMask;
    GPIOStruct& m_gpio;
};

}
