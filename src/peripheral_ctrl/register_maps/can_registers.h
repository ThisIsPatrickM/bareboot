#pragma once
#include "cortex_m/register_access/register.h"
#include "cortex_m/register_access/reserved_bytes.h"

#include <cstddef>

namespace RODOS {

namespace CAN_CNSTAT {
    using ST = RegSubValue<0, 4>;
    using PR = RegSubValue<4, 4>;
    using DLC = RegSubValue<12, 4>;
}

enum class CAN_ST_VALUE : uint8_t {
    RX_NOT_ACTIVE = 0,
    RX_READY = 2,
    RX_BUSY0 = 3,
    RX_FULL = 4,
    RX_BUSY1 = 5,
    RX_OVERRUN = 6,
    RX_BUSY2 = 7,
    TX_NOT_ACTIVE = 8,
    TX_RTR = 10,
    TX_ONCE = 12,
    TX_BUSY0 = 13,
    TX_ONCE_RTR = 14,
    TX_BUSY2 = 15
};


namespace CAN_CGCR {
    using CANEN = RegSubValue<0>;
    using CRX = RegSubValue<1>;
    using CTS = RegSubValue<2>;
    using BUFFLOCK = RegSubValue<3>;
    using TSTPEN = RegSubValue<4>;
    using DDIR = RegSubValue<5>;
    using LO = RegSubValue<6>;
    using IGNACK = RegSubValue<7>;
    using LOOPBACK = RegSubValue<8>;
    using INTERNAL = RegSubValue<9>;
    using DIAGEN = RegSubValue<10>;
    using EIT = RegSubValue<11>;
}

namespace CAN_CTIM {
    using TSEG2 = RegSubValue<0, 3>;
    using TSEG1 = RegSubValue<3, 4>;
    using SJW = RegSubValue<7, 2>;
    using PSC = RegSubValue<9, 7>;
}

namespace CAN_CIEN {
    using IEN = RegSubValue<0, 15>;
    using EIEN = RegSubValue<15>;
}

namespace CAN_CIPND {
    using IPND = RegSubValue<0, 15>;
    using EIPND = RegSubValue<15>;
}

namespace CAN_CICLR {
    using ICLR = RegSubValue<0, 15>;
    using EICLR = RegSubValue<15>;
}

namespace CAN_CICEN {
    using IEN = RegSubValue<0, 15>;
    using EIEN = RegSubValue<15>;
}

namespace CAN_CSTPND {
    using IST = RegSubValue<0, 4>;
    using IRQ = RegSubValue<4>;
    using NS = RegSubValue<5, 3>;
}

namespace CAN_CANEC {
    using TEC = RegSubValue<0, 8>;
    using REC = RegSubValue<8, 8>;
}

namespace CAN_CEDIAG {
    using EFID = RegSubValue<0, 4>;
    using EBID = RegSubValue<4, 6>;
    using TXE = RegSubValue<10>;
    using STUFF = RegSubValue<11>;
    using CRC = RegSubValue<12>;
    using MON = RegSubValue<13>;
    using DRIVE = RegSubValue<14>;
}


struct CANHardwareBuffer {
    Register CNSTAT;
    Register TSTP;
    Register DATA3;
    Register DATA2;
    Register DATA1;
    Register DATA0;
    Register ID0;
    Register ID1;
};

/**
 * Note: it's best to ignore Vorago's documentations
 * This chip has the same CAN Controller and much better documentation:
 * https://www.ti.com/lit/ds/symlink/cp3bt26.pdf
 */
struct CANStruct {
    static constexpr size_t REGISTERS_PER_BUFFER { 8 };
    static constexpr size_t NUM_BUFFERS { 15 };
    CANHardwareBuffer buffers[NUM_BUFFERS];
    ReservedWord hiddenBuffer[REGISTERS_PER_BUFFER];
    Register CGCR;
    Register CTIM;
    Register GMSKX;
    Register GMSKB;
    Register BMSKX;
    Register BMSKB;
    Register CIEN;
    Register CIPND;
    Register CICLR;
    Register CICEN;
    Register CSTPND;
    Register CANEC;
    Register CEDIAG;
    Register CTMR;
    ReservedWord RESERVED[(0x400-0x238)/sizeof(ReservedWord)];
};

static_assert(sizeof(CANStruct) == 0x400);

}

