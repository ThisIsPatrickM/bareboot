#pragma once

#include "bootloader.h"
#include "call_constructors.h"
#include "memory_map.h"
#include "metadata.h"
#include <cstdint>

namespace bootloader {

void Move_Vector_Table();

void Toggle_ROM_Writeable(bool writeable);

void Disable_Interrupts();

void Enable_Interrupts();

void Init_Watchdog();

// TODO Move to a class?

} // namespace bootloader
