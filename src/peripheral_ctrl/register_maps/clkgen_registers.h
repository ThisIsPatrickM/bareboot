#pragma once
#include "cortex_m/register_access/register.h"

namespace RODOS {

namespace CLKGEN_CTRL0 {
    using REF_CL_SEL = RegSubValue<0, 2>;
    using CLKSEL_SYS = RegSubValue<2, 2>;
    using PLL_INTFB = RegSubValue<4>;
    using PLL_PWDN = RegSubValue<5>;
    using PLL_BYPASS = RegSubValue<6>;
    using PLL_TEST = RegSubValue<7>;
    using PLL_BWADJ = RegSubValue<8, 6>;
    using PLL_CLKOD = RegSubValue<14, 4>;
    using PLL_CLKF = RegSubValue<18, 6>;
    using PLL_CLKR = RegSubValue<24, 4>;
    using CLK_DIV_SEL = RegSubValue<28,2>;
    using PLL_RESET = RegSubValue<30>;
    using SYS_CLK_LOST_DET_EN = RegSubValue<31>;
}

namespace CLKGEN_STAT {
    using FBSLIP = RegSubValue<0>;
    using RFSLIP = RegSubValue<1>;
    using LOCKLOST = RegSubValue<2>;
    using SYSCLKLOST = RegSubValue<3>;
}

namespace CLKGEN_CTRL1 {
    using SYS_CLK_LOST_DET_REARM = RegSubValue<0>;
    using PLL_LCK_DET_REARM = RegSubValue<1>;
    using PLL_LOST_LOCK_DET_EN = RegSubValue<2>;
    using XTAL_EN = RegSubValue<3>;
    using XTAL_N_EN = RegSubValue<4>;
    using ADC_CLK_DIV_SEL = RegSubValue<5>;
}

struct CLKGENStruct {
    Register CTRL0;
    Register STAT;
    Register CTRL1;
};

static_assert(sizeof(CLKGENStruct) == 0xC);

}

