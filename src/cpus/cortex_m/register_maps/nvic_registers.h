#pragma once
#include "cortex_m/register_access/register.h"

namespace RODOS {

namespace NVIC_IPR {
    using PRIO = RegSubValue8<4, 4>;
}

struct NVICStruct {
    Register ISER[16];
    ReservedWord RESERVED0[0x10];
    Register ICER[16];
    ReservedWord RESERVED1[0x10];
    Register ISPR[16];
    ReservedWord RESERVED2[0x10];
    Register ICPR[16];
    ReservedWord RESERVED3[0x10];
    Register IABR[16];
    ReservedWord RESERVED4[0x30];
    Register8 IPR[496];
    ReservedWord RESERVED5[0x1c4];
};

static_assert(sizeof(NVICStruct) == 0xC00);

}
