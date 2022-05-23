// #include "rodos-debug.h"
#include "hw_hal_gpio.h"
#include "peripheral_ctrl/peripheral_defs.h"

namespace RODOS {

HW_HAL_GPIO::HW_HAL_GPIO(pin_config::VA41620_PIN pinIdx, HAL_GPIO& owner)
    : m_idx { pinIdx }
    , m_owner { owner }
    , m_portNum { pinIdx.getPortNum() }
    , m_gpio { gpioStructs[m_portNum] }
{
}

int32_t HW_HAL_GPIO::init(bool isOutput, uint8_t numOfPins, uint32_t initVal)
{
    int returnValue = setNumPins(numOfPins);
    if (returnValue != 0) {
        return returnValue;
    }
    sysconfigPeripheralBitband->GPIO_PORT_ENABLE[m_portNum].write(1);
    if (isOutput) {
        setOutput(static_cast<uint16_t>(initVal));
        m_gpio.DIR.setBits(m_pinMask);
    }
    return returnValue;
}

int32_t HW_HAL_GPIO::setMode(GPIO::MODE mode)
{
    (void)mode;
    // RODOS_ERROR("setMode not implemented");
    return -1;
}

int32_t HW_HAL_GPIO::setNumPins(uint32_t numPins)
{
    uint32_t nOnes = (1ul << numPins) - 1;
    uint32_t pinMask = nOnes << (m_idx.getPinInPort());
    bool validPinmask = (pinMask & 0xFFFF'0000) == 0;
    m_pinMask = static_cast<uint16_t>(pinMask);
    return validPinmask ? 0 : -1;
}

int32_t HW_HAL_GPIO::setPull(pin_config::PULL pull)
{
    (void)pull;
    // RODOS_ERROR("setPull not implemented");
    return -1;
}

void HW_HAL_GPIO::reset()
{
    // RODOS_ERROR("reset not implemented");
}

void HW_HAL_GPIO::setOutput(uint16_t val)
{
    m_gpio.SETOUT.write(static_cast<uint32_t>((val << (m_idx.getPinInPort())) & m_pinMask));
    m_gpio.CLROUT.write(
        static_cast<uint32_t>((static_cast<uint16_t>(~val) << (m_idx.getPinInPort())) & m_pinMask));
}

uint16_t HW_HAL_GPIO::readInput()
{
    return static_cast<uint16_t>((m_gpio.DATAIN.read() & m_pinMask) >> m_idx.getPinInPort());
}

void HW_HAL_GPIO::enableInterrupts()
{
    // RODOS_ERROR("enableInterrupts not implemented");
}

void disableInterrupts()
{
    // RODOS_ERROR("DisableInterrupts not implemented");
}

}
