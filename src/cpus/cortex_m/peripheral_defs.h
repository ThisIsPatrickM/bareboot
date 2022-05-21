#pragma once
#include "register_maps/scb_registers.h"
#include "register_maps/nvic_registers.h"
#include "register_maps/systick_registers.h"


namespace RODOS {

inline SCBStruct& scb = *reinterpret_cast<SCBStruct*>(0xE000'ED00);
inline NVICStruct& nvic = *reinterpret_cast<NVICStruct*>(0xE000'E100);
inline SysTickStruct& systick = *reinterpret_cast<SysTickStruct*>(0xE000'E010);

}
