#include "microcontroller/microcontroller_dependent.h"

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
