#pragma once
#include "microcontroller/va41620/peripheral_ctrl/clock_management/clkgen_configstruct.h"
#include "microcontroller/va41620/peripheral_ctrl/clock_management/clkgen_setup.h"

namespace RODOS {

/**
 * before changing anything, please look at the datasheet for limits to these values
 */
constexpr ClkgenConfigStruct CLKGEN_CONFIG {
    .xtalMode = XtalMode::EXT_CLK,
    .xtalFreq = 40'000'000, // must be <=100MHz for EXT_CLK or >=4MHz and <=10MHz for XTAL
    .usePLL = true,
    .pllTargetFrequency = 100'000'000, // must be <= 100MHz
    .requireExactPLLFrequency = true,
    .sysclkSource = SysclkSource::PLL
};

constexpr ClockSetup GLOBAL_CLOCK_SETUP { ClockSetup::generateSetup<CLKGEN_CONFIG>() };

}
