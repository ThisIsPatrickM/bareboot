#include "rodos_includes/misc-rodos-funcs.h"

#include "hw_hal_gpio.h"
#include "rodos_includes/hal_gpio.h"
#include <new>

namespace RODOS {

HAL_GPIO::HAL_GPIO(GPIO_PIN pinIdx)
    : context { xmallocAndTrapOnFailure<HW_HAL_GPIO>() }
{
    new (context) HW_HAL_GPIO(pinIdx, *this);
}

int32_t HAL_GPIO::init(
    bool isOutput, uint8_t numOfPins, uint32_t initVal, GPIO_OUTPUT_TYPE outputType)
{
    if (outputType != GPIO_OUTPUT_TYPE::PUSH_PULL) {
        // RODOS_ERROR("Open drain output not supported!");
        return -1;
    }
    return context->init(isOutput, numOfPins, initVal);
}

int32_t HAL_GPIO::config(GPIO_CFG_TYPE type, uint32_t paramVal)
{
    switch (type) {
    case GPIO_CFG_OUTPUT_ENABLE:
        context->setMode(GPIO::MODE::OUTPUT);
        return 0;

    case GPIO_CFG_NUM_OF_PINS:
        context->setNumPins(paramVal);
        return 0;

    case GPIO_CFG_PULLUP_ENABLE:
        context->setPull(paramVal ? pin_config::PULL::UP : pin_config::PULL::OFF);
        return 0;

    case GPIO_CFG_PULLDOWN_ENABLE:
        context->setPull(paramVal ? pin_config::PULL::DOWN : pin_config::PULL::OFF);
        return 0;

    case GPIO_CFG_IRQ_SENSITIVITY:
        if (paramVal <= GPIO_IRQ_SENS_FALLING) {
            context->m_irqSensitivity = (RODOS::GPIO_IRQ_SENSITIVITY)paramVal;
            return 0;
        }
        return -1;

    default:
        return -1;
    }
}

void HAL_GPIO::reset()
{
    context->reset();
}

void HAL_GPIO::setPins(uint32_t val)
{
    context->setOutput(static_cast<uint16_t>(val));
}

uint32_t HAL_GPIO::readPins()
{
    return context->readInput();
}

bool HAL_GPIO::isDataReady()
{
    return context->m_interruptEventOccurred;
}

void HAL_GPIO::interruptEnable(bool enable)
{
    if (enable) {
        context->enableInterrupts();
    } else {
        context->disableInterrupts();
    }
}

void HAL_GPIO::resetInterruptEventStatus()
{
    context->m_interruptEventOccurred = false;
}

}
