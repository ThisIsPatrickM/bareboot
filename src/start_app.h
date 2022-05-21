#pragma once

#include <cstdint>

namespace bootloader {

extern "C" void Start_App(
    [[maybe_unused]] void* programmCounter, [[maybe_unused]] void* stackPointer);

} // namespace bootloader
