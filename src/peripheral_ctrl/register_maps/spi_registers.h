#pragma once
#include "cortex_m/register_access/register.h"
#include "cortex_m/register_access/reserved_bytes.h"

namespace RODOS {

namespace SPI_CTRL0 {
    using SIZE = RegSubValue<0, 4>;
    using SPO = RegSubValue<6>;
    using SPH = RegSubValue<7>;
    using SCRDV = RegSubValue<8, 8>;
}

namespace SPI_CTRL1 {
    using LBM = RegSubValue<0>;
    using ENABLE = RegSubValue<1>;
    using MS = RegSubValue<2>;
    using SOD = RegSubValue<3>;
    using SS = RegSubValue<4, 3>;
    using BLOCKMODE = RegSubValue<7>;
    using BMSTART = RegSubValue<8>;
    using BMSTALL = RegSubValue<9>;
    using MDLYCAP = RegSubValue<10>;
    using MTXCAP = RegSubValue<11>;    
}

namespace SPI_DATA {
    using VALUE = RegSubValue<0, 16>;
    using BMSKIPDATA = RegSubValue<30>;
    using BMSTART = RegSubValue<31>;
    using BMSTOP = RegSubValue<31>;
}

namespace SPI_STATUS {
    using TFE = RegSubValue<0>;
    using TNF = RegSubValue<1>;
    using RNE = RegSubValue<2>;
    using RFF = RegSubValue<3>;
    using BUSY = RegSubValue<4>;
    using RXDATAFIRST = RegSubValue<5>;
    using RXTRIGGER = RegSubValue<6>;
    using TXTRIGGER = RegSubValue<7>;
}

namespace SPI_CLKPRESCALE {
    using VALUE = RegSubValue<1, 7>;
}

namespace SPI_IRQ_ENB {
    using RORIM = RegSubValue<0>;
    using RTIM = RegSubValue<1>;
    using RXIM = RegSubValue<2>;
    using TXIM = RegSubValue<3>;
}

namespace SPI_FIFO_CLR {
    using RXFIFO = RegSubValue<0>;
    using TXFIFO = RegSubValue<1>;
}

struct SPIStruct {
    Register CTRL0;
    Register CTRL1;
    Register DATA;
    Register STATUS;
    Register CLKPRESCALE;
    Register IRQ_ENB;
    Register IRQ_RAW;
    Register IRQ_END;
    Register IRQ_CLR;
    Register RXFIFOIRQTRG;
    Register TXFIFOIRQTRG;
    Register FIFO_CLR;
    Register STATE;
    ReservedWord RESERVED[(0x3F8-0x30)/sizeof(ReservedWord)];
    Register PERID;
};

static_assert(sizeof(SPIStruct) == 0x400);

}
