#pragma once
#include "microcontroller/va41620/cortex_m/hw_periph_bitband.h"
// #include "register_maps/clkgen_registers.h"
#include "register_maps/spi_registers.h"
#include "register_maps/sysconfig_peripheral_bitband.h"

namespace rodos {

inline SPIStruct* const SPIS = reinterpret_cast<SPIStruct*>(0x4001'5000); // NOLINT
// inline CLKGENStruct* const CLKGEN = reinterpret_cast<CLKGENStruct*>(0x4000'6000); // NOLINT
inline SYSCONFIGPeripheralBitbandStruct* const SYSCONFIG_PERIPHERAL_BITBAND =
    HWPeriphBitBand<0x4001'0050, 0, SYSCONFIGPeripheralBitbandStruct>::asObject;

}
