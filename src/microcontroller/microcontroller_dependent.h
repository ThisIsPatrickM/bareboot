#pragma once

#include "memory_map.h"
#include <cstdint>

namespace bootloader {

void Move_Vector_Table();

void Toggle_ROM_Writeable(bool writeable);

// TODO Init Watchdog?

} // namespace bootloader
