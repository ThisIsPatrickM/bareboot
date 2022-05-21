#include "pin_config.h"
#include "peripheral_ctrl/peripheral_defs.h"

namespace RODOS::pin_config {

void enableIOConfigPeripheral()
{
    sysconfigPeripheralBitband->IOCONFIG_ENABLE.write(1);
}

void configurePin(VA41620_PIN pin, PIN_FUNCTION function, OUTPUT_TYPE type, PULL pull)
{
    using namespace IOCONFIG_PINx;
    if (pin.isValid()) {
        ioconfig->PINx[pin].write(
            FUNSEL(static_cast<uint32_t>(function)),
            OPENDRN(static_cast<uint32_t>(type)),
            PEN((pull != PULL::OFF) ? 1 : 0),
            PLEVEL((pull == PULL::UP ? 1 : 0)));
    }
}
void resetPin(VA41620_PIN pin)
{
    configurePin(pin, PIN_FUNCTION::FUN0);
}
}
