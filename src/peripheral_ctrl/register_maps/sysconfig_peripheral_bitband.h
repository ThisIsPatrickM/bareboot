#pragma once
#include "cortex_m/register_access/register.h"
#include "cortex_m/register_access/reserved_bytes.h"

namespace RODOS {

/**
 * represents the TIM_RESETS, TIM_CLK_ENABLES,
 * PERIPHERAL_RESET and PERIPHERAL_CLK_ENABLE
 * registers in the bit-band region of the L4
 */
struct SYSCONFIGPeripheralBitbandStruct {
    Register TIM_RESET[24];
    ReservedWord RESERVED0[8];
    Register TIM_ENABLE[24];
    ReservedWord RESERVED1[8];
    Register SPI_RESET[4];
    Register UART_RESET[3];
    Register I2C_RESET[3];
    Register CAN_RESET[2];
    Register RNG_RESET;
    Register ADC_RESET;
    Register DAC_RESET;
    Register DMA_RESET;
    Register EBI_RESET;
    Register ETH_RESET;
    Register SPW_RESET;
    Register CLKGEN_RESET;
    Register IRQ_ROUTER_RESET;
    Register IOCONFIG_RESET;
    Register UTILITY_RESET;
    Register WATCHDOG_RESET;
    Register GPIO_PORT_RESET[7];
    ReservedWord RESERVED2;
    Register SPI_ENABLE[4];
    Register UART_ENABLE[3];
    Register I2C_ENABLE[3];
    Register CAN_ENABLE[2];
    Register RNG_ENABLE;
    Register ADC_ENABLE;
    Register DAC_ENABLE;
    Register DMA_ENABLE;
    Register EBI_ENABLE;
    Register ETH_ENABLE;
    Register SPW_ENABLE;
    Register CLKGEN_ENABLE;
    Register IRQ_ROUTER_ENABLE;
    Register IOCONFIG_ENABLE;
    Register UTILITY_ENABLE;
    Register WATCHDOG_ENABLE;
    Register GPIO_PORT_ENABLE[7];
    ReservedWord RESERVED3;
};

static_assert(sizeof(SYSCONFIGPeripheralBitbandStruct) == 4*32*sizeof(Register));

}    
