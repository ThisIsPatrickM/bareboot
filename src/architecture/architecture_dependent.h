#pragma once

#include "memory_map.h"

namespace bootloader {

void Start_App([[maybe_unused]] void* programmCounter, [[maybe_unused]] void* stackPointer);

} // namespace bootloader
