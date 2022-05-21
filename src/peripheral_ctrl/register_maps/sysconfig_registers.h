#pragma once
#include "cortex_m/register_access/register.h"

namespace RODOS {


namespace SYSCONFIG_RST_STAT {
    using POR = RegSubValue<0>;
    using EXTRST = RegSubValue<1>;
    using SYSRSTREQ = RegSubValue<2>;
    using LOCKUP = RegSubValue<3>;
    using WATCHDOG = RegSubValue<4>;
    using MEMERR = RegSubValue<5>;
}

namespace SYSCONFIG_RST_CNTL_ROM {
    using POR = RegSubValue<0>;
    using EXTRST = RegSubValue<1>;
    using SYSRSTREQ = RegSubValue<2>;
    using LOCKUP = RegSubValue<3>;
    using WATCHDOG = RegSubValue<4>;
    using MEMERR = RegSubValue<5>;
}

namespace SYSCONFIG_RST_CNTL_RAMx {
    using POR = RegSubValue<0>;
    using EXTRST = RegSubValue<1>;
    using SYSRSTREQ = RegSubValue<2>;
    using LOCKUP = RegSubValue<3>;
    using WATCHDOG = RegSubValue<4>;
    using MEMERR = RegSubValue<5>;
}

namespace SYSCONFIG_ROM_PROT {
    using WREN = RegSubValue<0>;
}

namespace SYSCONFIG_ROM_SCRUB {
    using VALUE = RegSubValue<0, 16>;
    using RESET = RegSubValue<31>;
}

namespace SYSCONFIG_RAMx_SCRUB {
    using VALUE = RegSubValue<0, 16>;
    using RESET = RegSubValue<31>;
}

namespace SYSCONFIG_IRQ_ENB {
    using ROMMBE = RegSubValue<0>;
    using ROMSBE = RegSubValue<1>;
    using RAM0MBE = RegSubValue<2>;
    using RAM0SBE = RegSubValue<3>;
    using RAM1MBE = RegSubValue<4>;
    using RAM1SBE = RegSubValue<5>;
}
namespace SYSCONFIG_IRQ_RAW {
    using ROMMBE = RegSubValue<0>;
    using ROMSBE = RegSubValue<1>;
    using RAM0MBE = RegSubValue<2>;
    using RAM0SBE = RegSubValue<3>;
    using RAM1MBE = RegSubValue<4>;
    using RAM1SBE = RegSubValue<5>;
}
namespace SYSCONFIG_IRQ_END {
    using ROMMBE = RegSubValue<0>;
    using ROMSBE = RegSubValue<1>;
    using RAM0MBE = RegSubValue<2>;
    using RAM0SBE = RegSubValue<3>;
    using RAM1MBE = RegSubValue<4>;
    using RAM1SBE = RegSubValue<5>;
}
namespace SYSCONFIG_IRQ_CLR {
    using ROMMBE = RegSubValue<0>;
    using ROMSBE = RegSubValue<1>;
    using RAM0MBE = RegSubValue<2>;
    using RAM0SBE = RegSubValue<3>;
    using RAM1MBE = RegSubValue<4>;
    using RAM1SBE = RegSubValue<5>;
}

namespace SYSCONFIG_RAMx_SBE {
    using COUNT = RegSubValue<0, 16>;
}
namespace SYSCONFIG_RAMx_MBE {
    using COUNT = RegSubValue<0, 16>;
}
namespace SYSCONFIG_ROM_SBE {
    using COUNT = RegSubValue<0, 16>;
}
namespace SYSCONFIG_ROM_MBE {
    using COUNT = RegSubValue<0, 16>;
}

namespace SYSCONFIG_ROM_RETRIES {
    using COUNT = RegSubValue<0, 16>;
}

namespace SYSCONFIG_REFRESH_CONFIG_H {
    using DIVCOUNT_H = RegSubValue<0, 8>;
    using TESTMODE = RegSubValue<30, 2>;
}

namespace SYSCONFIG_PERIPHERAL_RESET {
    using SPI0 = RegSubValue<0>;
    using SPI1 = RegSubValue<1>;
    using SPI2 = RegSubValue<2>;
    using SPI3 = RegSubValue<3>;
    using UART0 = RegSubValue<4>;
    using UART1 = RegSubValue<5>;
    using UART2 = RegSubValue<6>;
    using I2C0 = RegSubValue<7>;
    using I2C1 = RegSubValue<8>;
    using I2C2 = RegSubValue<9>;
    using CAN0 = RegSubValue<10>;
    using CAN1 = RegSubValue<11>;
    using RNG = RegSubValue<12>;
    using ADC = RegSubValue<13>;
    using DAC = RegSubValue<14>;
    using DMA = RegSubValue<15>;
    using EBI = RegSubValue<16>;
    using ETH = RegSubValue<17>;
    using SPW = RegSubValue<18>;
    using CLKGEN = RegSubValue<19>;
    using IRQ_ROUTER = RegSubValue<20>;
    using IOCONFIG = RegSubValue<21>;
    using UTILITY = RegSubValue<22>;
    using Watchdog = RegSubValue<23>;
    using PORTx = RegSubValue<24, 7>;
}

namespace SYSCONFIG_PERIPHERAL_CLK_ENABLE {
    using SPIx = RegSubValue<0, 4>;
    using UARTx = RegSubValue<4, 3>;
    using I2Cx = RegSubValue<7, 3>;
    using CANx = RegSubValue<10, 2>;
    using RNG = RegSubValue<12>;
    using ADC = RegSubValue<13>;
    using DAC = RegSubValue<14>;
    using DMA = RegSubValue<15>;
    using EBI = RegSubValue<16>;
    using ETH = RegSubValue<17>;
    using SPW = RegSubValue<18>;
    using CLKGEN = RegSubValue<19>;
    using IRQ_ROUTER = RegSubValue<20>;
    using IOCONFIG = RegSubValue<21>;
    using UTILITY = RegSubValue<22>;
    using Watchdog = RegSubValue<23>;
    using PORTx = RegSubValue<24, 7>;
}

namespace SYSCONFIG_SPW_M4_CTRL {
    using SPW_REG_EN = RegSubValue<15>;
    using LREN = RegSubValue<17>;
}

namespace SYSCONFIG_PMU_CTRL {
    using LVL_SLCT = RegSubValue<0, 2>;
}

namespace SYSCONFIG_WAKEUP_CNT {
    using WKUP_CNT = RegSubValue<0, 3>;
    using CNTSTRT = RegSubValue<3>;
}

namespace SYSCONFIG_EBI_CFGx {
    using ADDR_LOW = RegSubValue<0, 8>;
    using ADDR_HIGH = RegSubValue<8, 8>;
    using CFGREADCYCLE = RegSubValue<16, 3>;
    using CFGWRITECYCLE = RegSubValue<19, 3>;
    using CFGTURNAROUNDCYCLE = RegSubValue<22, 3>;
    using CFGSIZE = RegSubValue<25>;
}

namespace SYSCONFIG_EF_CONFIG {
    using ROM_SPEED = RegSubValue<0, 2>;
    using ROM_SIZE = RegSubValue<2, 4>;
    using ROM_NOCHECK = RegSubValue<6>;
    using BOOT_DELAY = RegSubValue<7, 3>;
    using ROM_READ = RegSubValue<10, 8>;
    using ROM_LATENCY = RegSubValue<18, 5>;
    using ROM_ADDRESS = RegSubValue<23, 2>;
    using ROM_DLYCAP = RegSubValue<25>;
    using ROM_STATUS = RegSubValue<26>;
    using RM = RegSubValue<27>;
    using WM = RegSubValue<28>;
}



struct SYSCONFIGStruct {
    Register RST_STAT;
    Register RST_CNTL_ROM;
    Register RST_CNTL_RAM0;
    Register RST_CNTL_RAM1;
    Register ROM_PROT;
    Register ROM_SCRUB;
    Register RAM0_SCRUB;
    Register RAM1_SCRUB;
    Register IRQ_ENB;
    Register IRQ_RAW;
    Register IRQ_END;
    Register IRQ_CLR;
    Register RAM0_SBE;
    Register RAM1_SBE;
    Register RAM0_MBE;
    Register RAM1_MBE;
    Register ROM_SBE;
    Register ROM_MBE;
    Register ROM_RETRIES;
    Register REFRESH_CONFIG_H;
    Register TIM_RESETS;
    Register TIM_CLK_ENABLES;
    Register PERIPHERAL_RESET;
    Register PERIPHERAL_CLK_ENABLE;
    Register SPW_M4_CTRL;
    Register PMU_CTRL;
    Register WAKEUP_CNT;
    Register EBI_CFG0;
    Register EBI_CFG1;
    Register EBI_CFG2;
    Register EBI_CFG3;
    Register ANALOG_CNTL;
    Register SW_CLKDIV10;
    Register REFRESH_CONFIG_L;
    ReservedWord RESERVED0[(0xFEC-0x088)/4];
    Register EF_CONFIG;
    Register EF_ID0;
    Register EF_ID1;
    Register PROCID;
    Register PERID;
};

static_assert(sizeof(SYSCONFIGStruct) == 0x1000);

}
