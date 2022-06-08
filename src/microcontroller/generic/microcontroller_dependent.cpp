#include "microcontroller/microcontroller_dependent.h"

/* Controller specific includes */
#include <iostream>

/* Regular Implementations */
namespace bootloader {
void Move_Vector_Table() {}

void Toggle_ROM_Writeable([[maybe_unused]] bool writeable) {}

void Disable_Interrupts() {}

void Enable_Interrupts() {}

void Init_Watchdog()
{
    // TODO
}

} // namespace bootloader

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) // NOLINT
{
    bootloader::Bootloader boot {};
    std::cout << "Starting bootloader" << std::endl;
    boot.run();
    return 0;
}
