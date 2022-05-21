#pragma once
#include "cortex_m/register_access/register.h"
#include "cortex_m/register_access/reserved_bytes.h"

namespace RODOS {

namespace I2C_CTRL {
    using CLKENABLED = RegSubValue<0>;
    using ENABLED = RegSubValue<1>;
    using ENABLE = RegSubValue<2>;
    using TXFEMD = RegSubValue<3>;
    using RXFFMD = RegSubValue<4>;
    using ALGFILTER = RegSubValue<5>;
    using DLGFILTER = RegSubValue<6>;
    using LOOPBACK = RegSubValue<8>;
    using TMCONFIGENB = RegSubValue<9>;
}

namespace I2C_CLKSCALE {
    using VALUE = RegSubValue<0, 8>;
    using FASTMODE = RegSubValue<31>;
}

namespace I2C_ADDRESS {
    using DIRECTION = RegSubValue<0>;
    using ADDRESS = RegSubValue<1, 10>;
    using A10MODE = RegSubValue<15>;

    constexpr uint8_t I2C_DIRECTION_TX = 0;
    constexpr uint8_t I2C_DIRECTION_RX = 1;
}

namespace I2C_CMD {
    using START = RegSubValue<0>;
    using STOP = RegSubValue<1>;
    using CANCEL = RegSubValue<2>;
}

namespace I2C_STATUS {
    using I2CIDLE = RegSubValue<0>;
    using IDLE = RegSubValue<1>;
    using WAITING = RegSubValue<2>;
    using STALLED = RegSubValue<3>;
    using ARBLOST = RegSubValue<4>;
    using NACKADDR = RegSubValue<5>;
    using NACKDATA = RegSubValue<6>;
    using RXNEMPTY = RegSubValue<8>;
    using RXFULL = RegSubValue<9>;
    using RXTRIGGER = RegSubValue<11>;
    using TXEMPTY = RegSubValue<12>;
    using TXNFULL = RegSubValue<13>;
    using TXTRIGGER = RegSubValue<15>;
    using RAW_SDA = RegSubValue<30>;
    using RAW_SCL = RegSubValue<31>;
}

namespace I2C_IRQ_ENB {
    using STATUS = RegSubValue<0, 7>;
    using CLKLOTO = RegSubValue<7>;
    using TXOVERFLOW = RegSubValue<10>;
    using RXOVERFLOW = RegSubValue<11>;
    using TXREADY = RegSubValue<12>;
    using RXREADY = RegSubValue<13>;
    using TXEMPTY = RegSubValue<14>;
    using RXFULL = RegSubValue<15>;
}

namespace I2C_FIFO_CLR {
    using RXFIFO = RegSubValue<0>;
    using TXFIFO = RegSubValue<1>;
}

struct I2CStruct {
    Register CTRL;
    Register CLKSCALE;
    Register WORDS;
    Register ADDRESS;
    Register DATA;
    Register CMD;
    Register STATUS;
    Register STATE;
    Register TXCOUNT;
    Register RXCOUNT;
    Register IRQ_ENB;
    Register IRQ_RAW;
    Register IRQ_END;
    Register IRQ_CLR;
    Register RXFIFOIRQTRG;
    Register TXFIFOIRQTRG;
    Register FIFO_CLR;
    Register TMCONFIG;
    Register CLKTOLIMIT;
    ReservedWord RESERVED0[(0x100-0x4C)/sizeof(ReservedWord)];
    Register S0_CTRL;
    Register S0_MAXWORDS;
    Register S0_ADDRESS;
    Register S0_ADDRESSMASK;
    Register S0_DATA;
    Register S0_LASTADDRESS;
    Register S0_STATUS;
    Register S0_STATE;
    Register S0_TXCOUNT;
    Register S0_RXCOUNT;
    Register S0_IRQ_ENB;
    Register S0_IRQ_RAW;
    Register S0_IRQ_END;
    Register S0_IRQ_CLR;
    Register S0_RXFIFOIRQTRG;
    Register S0_TXFIFOIRQTRG;
    Register S0_FIFO_CLR;
    Register S0_ADDRESSB;
    Register S0_ADDRESSMASKB;
    ReservedWord RESERVED1[(0x3FC-0x14C)/sizeof(ReservedWord)];
    Register PERID;
};

static_assert(sizeof(I2CStruct)==0x400);

}
