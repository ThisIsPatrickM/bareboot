#pragma once
#include "cortex_m/register_access/register.h"

namespace RODOS{

namespace SysTick_CSR {
    using ENABLE = RegSubValue<0>;
    using TICKINT = RegSubValue<1>;
    using CLKSOURCE = RegSubValue<2>;
    using COUNTFLAG = RegSubValue<16>;
}

struct SysTickStruct {
    Register CSR;
    Register RVR;
    Register CVR;
    Register CALIB;
};

}
