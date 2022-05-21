#pragma once
#include <cstdint>

namespace RODOS {

enum class XTAL_MODE {
    OFF = 0,
    XTAL = 1,
    EXT_CLK = 2
};
    
enum class PLL_SOURCE {
    NONE = 0,
    XTAL = 1,
    EXT_CLK = 2,
};
    
enum class SYSCLK_SOURCE {
    HBO = 0,
    EXT_CLK = 1,
    PLL = 2,
    XTAL = 3
};

class CLKGENConfigStruct {
public:
    const XTAL_MODE xtalMode;
    const int32_t xtalFreq; ///< must be <=100MHz for EXT_CLK or >=4MHz and <=10MHz for XTAL

    const bool usePLL;
    const int32_t pllTargetFrequency; ///< must be <=100MHz
    const bool requireExactPLLFrequency;

    const SYSCLK_SOURCE sysclkSource;
};

}
