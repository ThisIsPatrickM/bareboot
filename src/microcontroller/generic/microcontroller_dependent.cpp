#include "microcontroller/microcontroller_dependent.h"

/* Controller specific includes */
#include <iostream>

/* Regular Implementations */
namespace bootloader {
void Move_Vector_Table() {}

void Disable_Interrupts() {}

void Enable_Interrupts() {}

void Init_Watchdog()
{
    // TODO
}

} // namespace bootloader

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) // NOLINT
{
    std::cout << "Starting bootloader" << std::endl;
    bootloader::Bootloader boot {};
    boot.run();
    return 0;
}
