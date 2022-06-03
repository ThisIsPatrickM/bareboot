#pragma once
#include <cstdint>

namespace rodos {

enum class XtalMode { OFF = 0, XTAL = 1, EXT_CLK = 2 };

enum class PllSource {
    NONE = 0,
    XTAL = 1,
    EXT_CLK = 2,
};

enum class SysclkSource { HBO = 0, EXT_CLK = 1, PLL = 2, XTAL = 3 };

class ClkgenConfigStruct {
public:
    const XtalMode xtalMode;
    const int32_t xtalFreq; ///< must be <=100MHz for EXT_CLK or >=4MHz and <=10MHz for XTAL

    const bool usePLL;
    const int32_t pllTargetFrequency; ///< must be <=100MHz
    const bool requireExactPLLFrequency;

    const SysclkSource sysclkSource;
};

}
