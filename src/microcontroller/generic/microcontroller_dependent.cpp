#include "microcontroller/microcontroller_dependent.h"

namespace bootloader {
void Move_Vector_Table() {}

void Init_Watchdog() {}

void Trigger_Watchdog() {}

} // namespace bootloader

// int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) // NOLINT
// {
//     std::cout << "Starting bootloader" << std::endl;
//     bootloader::Bootloader boot {};
//     boot.run();
//     return 0;
// }
