#pragma once

#include "startup.h"
#include <cstdint>

namespace bootloader {

void Start_App([[maybe_unused]] void* programmCounter, [[maybe_unused]] void* stackPointer);

} // namespace bootloader
