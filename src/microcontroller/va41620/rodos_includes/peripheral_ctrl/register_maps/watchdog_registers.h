#pragma once
#include "microcontroller/va41620/rodos_includes/cortex_m/register_access/register.h"
#include "microcontroller/va41620/rodos_includes/cortex_m/register_access/reserved_bytes.h"

namespace RODOS {

// NOLINTBEGIN
namespace WD_WDOGCONTROL {
    using RESEN = RegSubValue<1>;
    using INTEN = RegSubValue<0>;
}

struct WatchdogStruct {
    Register WDOGLOAD;
    Register WDOGVALUE;
    Register WDOGCONTROL;
    Register WDOGINTCLR;
    Register WDOGRIS;
    Register WDOGMIS;
    ReservedWord RESERVED0[(0xC00 - 0x18) / sizeof(ReservedWord)];
    Register WDOGLOCK;
    ReservedWord RESERVED1[(0xF00 - 0xC04) / sizeof(ReservedWord)];
    Register WDOGITCR;
    Register WDOGITOP;
    ReservedWord RESERVED2[(0xFE0 - 0xF08) / sizeof(ReservedWord)];
    ReservedWord WDOGPERIPHID0;
    ReservedWord WDOGPERIPHID1;
    ReservedWord WDOGPERIPHID2;
    ReservedWord WDOGPERIPHID3;
    ReservedWord WDOGPCELLID0;
    ReservedWord WDOGPCELLID1;
    ReservedWord WDOGPCELLID2;
    ReservedWord WDOGPCELLID3;
};

static_assert(sizeof(WatchdogStruct) == 0x1000);

// NOLINTEND

}
