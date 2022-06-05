#pragma once

#include "memory_map.h"
#include <cstdint>

namespace bootloader {

void Move_Vector_Table();

void Toggle_ROM_Writeable(bool writeable);

void Disable_Interrupts();

void Enable_Interrupts();

void Init_Watchdog();

// TODO Move to a class?

} // namespace bootloader
