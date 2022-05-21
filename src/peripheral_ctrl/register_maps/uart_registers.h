#pragma once

#include "cortex_m/register_access/register.h"
#include "cortex_m/register_access/reserved_bytes.h"

namespace RODOS {

namespace UART_DATA {
    using VALUE = RegSubValue<0, 8>;
    using DPARITY = RegSubValue<15>;
}

namespace UART_ENABLE {
    using RXENABLE = RegSubValue<0>;
    using TXENABLE = RegSubValue<1>;
}

namespace UART_CTRL {
    using PAREN = RegSubValue<0>;
    using PAREVEN = RegSubValue<1>;
    using PARMAN = RegSubValue<2>;
    using STOPBITS = RegSubValue<3>;
    using WORDSIZE = RegSubValue<4,2>;
    using LOOPBACK = RegSubValue<6>;
    using LOOPBACKBLK = RegSubValue<7>;
    using AUTOCTS = RegSubValue<8>;
    using DEFRTS = RegSubValue<9>;
    using AUTORTS = RegSubValue<10>;
    using BAUD8 = RegSubValue<11>;
}

namespace UART_CLKSCALE {
    using FRAC = RegSubValue<0, 6>;
    using INT = RegSubValue<6, 18>;
    /**
     * not in the manual, but the FRAC and INT fields can just be viewed as a combined fixed-point value
     */
    using COMBINEDVALUE = RegSubValue<0, 24>;
}

namespace UART_RXSTATUS {
    using RDAVL = RegSubValue<0>;
    using RDNFULL = RegSubValue<1>;
    using RXBUSY = RegSubValue<2>;
    using RXTO = RegSubValue<3>;
    using RXOVR = RegSubValue<4>;
    using RXFRM = RegSubValue<5>;
    using RXPAR = RegSubValue<6>;
    using RXBRK = RegSubValue<7>;
    using RXBUSYBRK = RegSubValue<8>;
    using RXADDR9 = RegSubValue<9>;
    using RXRTSN = RegSubValue<15>;
}

namespace UART_TXSTATUS {
    using WRRDY = RegSubValue<0>;
    using WRBUSY = RegSubValue<1>;
    using TXBUSY = RegSubValue<2>;
    using WRLOST = RegSubValue<3>;
    using TXCTSN = RegSubValue<15>;
}

namespace UART_FIFO_CLR {
    using RXFIFO = RegSubValue<0>;
    using TXFIFO = RegSubValue<1>;
}

namespace UART_TBREAK {
    using BRKCNT = RegSubValue<0, 7>;
}

namespace UART_ADDR9 {
    using DATA = RegSubValue<0, 8>;
    using ENB9BIT = RegSubValue<15>;
}

namespace UART_IRQ_ENB {
    using IRQ_RX = RegSubValue<0>;
    using IRQ_RX_STATUS = RegSubValue<1>;
    using IRQ_RX_TO = RegSubValue<2>;
    using IRQ_TX = RegSubValue<4>;
    using IRQ_TX_STATUS = RegSubValue<5>;
    using IRQ_TX_EMPTY = RegSubValue<6>;
    using IRQ_TX_CTS= RegSubValue<7>;
}

struct UARTStruct {
    Register DATA;
    Register ENABLE;
    Register CTRL;
    Register CLKSCALE;
    Register RXSTATUS;
    Register TXSTATUS;
    Register FIFO_CLR;
    Register TXBREAK;
    Register ADDR9;
    Register ADDR9MASK;
    Register IRQ_ENB;
    Register IRQ_RAW;
    Register IRQ_END;
    Register IRQ_CLR;
    Register RXFIFOIRQTRG;
    Register TXFIFOIRQTRG;
    Register RXFIFORTSTRG;
    Register STATE;
    ReservedWord RESERVED0[(0xFFC-0x48)/sizeof(Register)];
    Register PERID;
};

static_assert(sizeof(UARTStruct)==0x1000);

}
