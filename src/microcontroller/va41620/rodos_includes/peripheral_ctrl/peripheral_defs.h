#pragma once

#include "microcontroller/va41620/rodos_includes/cortex_m/hw_periph_bitband.h"
#include "register_maps/spi_registers.h"
#include "register_maps/sysconfig_peripheral_bitband.h"
#include "register_maps/sysconfig_registers.h"
#include "register_maps/watchdog_registers.h"

namespace RODOS {

inline SPIStruct* const SPIS = reinterpret_cast<SPIStruct*>(0x4001'5000); // NOLINT
inline SYSCONFIGStruct* const SYSCONFIG = reinterpret_cast<SYSCONFIGStruct*>(0x4001'0000); // NOLINT
inline WatchdogStruct* const WATCHDOG = reinterpret_cast<WatchdogStruct*>(0x4002'1000); // NOLINT
inline SYSCONFIGPeripheralBitbandStruct* const SYSCONFIG_PERIPHERAL_BITBAND =
    HWPeriphBitBand<0x4001'0050, 0, SYSCONFIGPeripheralBitbandStruct>::asObject;

}
