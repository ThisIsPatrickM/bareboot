#pragma once
#include "peripheral_ctrl/pin_config/enums.h"
#include "peripheral_ctrl/register_maps/ioconfig_registers.h"
#include "rodos_includes/hal_gpio.h"

namespace RODOS::pin_config {

void enableIOConfigPeripheral();
void configurePin(
    VA41620_PIN pin,
    PIN_FUNCTION function,
    OUTPUT_TYPE type = OUTPUT_TYPE::PUSHPULL,
    PULL pull = PULL::OFF);
void resetPin(VA41620_PIN pin);

}
