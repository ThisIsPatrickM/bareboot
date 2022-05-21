#pragma once

#include "cortex_m/register_access/register.h"
#include "cortex_m/register_access/reserved_bytes.h"

namespace RODOS {

struct GPIOStruct {
    Register DATAIN;
    Register DATAINRAW;
    Register DATAOUT;
    Register DATAOUTRAW;
    Register SETOUT;
    Register CLROUT;
    Register TOGOUT;
    Register DATAMASK;
    Register DIR;
    Register PULSE;
    Register PULSEBASE; 
    Register DELAY1;
    Register DELAY2;
    Register IRQ_SEN;
    Register IRQ_EDGE;
    Register IRQ_EVT;
    Register IRQ_ENB;
    Register IRQ_RAW;
    Register IRQ_END;
    Register EDGE_STATUS;
    ReservedWord RESERVED0[(0x3FC-0x050)/4];
    Register PERID;
};
static_assert(sizeof(GPIOStruct)==0x400);

}
