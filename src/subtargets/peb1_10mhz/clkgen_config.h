#pragma once
#include "peripheral_ctrl/clock_management/clkgen_configstruct.h"
#include "peripheral_ctrl/clock_management/clkgen_setup.h"

namespace RODOS {

/**
 * before changing anything, please look at the datasheet for limits to these values
 */
constexpr CLKGENConfigStruct clkgenConfig {
    .xtalMode = XTAL_MODE::EXT_CLK,
    .xtalFreq = 40'000'000, //must be <=100MHz for EXT_CLK or >=4MHz and <=10MHz for XTAL
    .usePLL = true,
    .pllTargetFrequency = 10'000'000, //must be <= 100MHz
    .requireExactPLLFrequency = true,
    .sysclkSource = SYSCLK_SOURCE::PLL
};

constexpr ClockSetup globalClockSetup { ClockSetup::generateSetup<clkgenConfig>() };

}
