#pragma once
#include "cortex_m/register_access/register.h"
#include "cortex_m/register_access/reserved_bytes.h"

namespace RODOS {

namespace IOCONFIG_PINx {
    using FLTTYPE = RegSubValue<0, 3>;
    using FLTCLK = RegSubValue<3, 3>;
    using INVINP = RegSubValue<6>;
    using IEWO = RegSubValue<7>;
    using OPENDRN = RegSubValue<8>;
    using INVOUT = RegSubValue<9>;
    using PLEVEL = RegSubValue<10>;
    using PEN = RegSubValue<11>;
    using PWOA = RegSubValue<12>;
    using FUNSEL = RegSubValue<13, 2>;
    using IODIS = RegSubValue<16>;
}

constexpr uint32_t TOTAL_NUMBER_OF_PINS {104};

struct IOCONFIGStruct {
    Register PINx[TOTAL_NUMBER_OF_PINS];
    ReservedWord RESERVED0[(0x1C0-0x1A0)/sizeof(Register)];
    Register CLKDIV[8];
    ReservedWord RESERVED1[(0xFFC-0x1E0)/sizeof(Register)];
    Register PERID;
};

static_assert(sizeof(IOCONFIGStruct)==0x1000);

}
