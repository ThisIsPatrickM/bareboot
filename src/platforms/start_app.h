#pragma once

#include "memory_map.h"
#include <cstdint>

namespace bootloader {

void Start_App([[maybe_unused]] void* programmCounter, [[maybe_unused]] void* stackPointer);

void Move_Vector_Table();

} // namespace bootloader
