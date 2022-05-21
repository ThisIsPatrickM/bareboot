#pragma once

#include "cortex_m/hw_periph_bitband.h"
#include "register_maps/adc_registers.h"
#include "register_maps/can_registers.h"
#include "register_maps/clkgen_registers.h"
#include "register_maps/gpio_registers.h"
#include "register_maps/i2c_registers.h"
#include "register_maps/ioconfig_registers.h"
#include "register_maps/spi_registers.h"
#include "register_maps/sysconfig_peripheral_bitband.h"
#include "register_maps/sysconfig_registers.h"
#include "register_maps/tim_registers.h"
#include "register_maps/uart_registers.h"

namespace RODOS {

inline ADCStruct* const adc = reinterpret_cast<ADCStruct*>(0x4002'2000);
inline CANStruct* const cans = reinterpret_cast<CANStruct*>(0x4001'4000);
inline SPIStruct* const spis = reinterpret_cast<SPIStruct*>(0x4001'5000);
inline IOCONFIGStruct* const ioconfig = reinterpret_cast<IOCONFIGStruct*>(0x4001'1000);
inline I2CStruct* const i2cs = reinterpret_cast<I2CStruct*>(0x4001'6000);
inline UARTStruct* const uart0 = reinterpret_cast<UARTStruct*>(0x4002'4000);
inline UARTStruct* const uart1 = reinterpret_cast<UARTStruct*>(0x4001'7000);
inline UARTStruct* const uart2 = reinterpret_cast<UARTStruct*>(0x4002'4000);
inline TIMStruct* const tim0to15 = reinterpret_cast<TIMStruct*>(0x4001'8000);
inline TIMStruct* const tim16to23 = reinterpret_cast<TIMStruct*>(0x4002'8000);
inline CLKGENStruct* const clkgen = reinterpret_cast<CLKGENStruct*>(0x4000'6000);
inline SYSCONFIGStruct* const sysconfig = reinterpret_cast<SYSCONFIGStruct*>(0x4001'0000);
inline GPIOStruct* const gpioStructs = reinterpret_cast<GPIOStruct*>(0x4001'2000);
inline SYSCONFIGPeripheralBitbandStruct* const sysconfigPeripheralBitband =
    HWPeriphBitBand<0x4001'0050, 0, SYSCONFIGPeripheralBitbandStruct>::asObject;

}
