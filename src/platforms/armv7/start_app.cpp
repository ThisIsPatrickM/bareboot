#include "../../start_app.h"

namespace bootloader {

extern "C" void Start_App(
    [[maybe_unused]] void* programmCounter, [[maybe_unused]] void* stackPointer)
{
    // TODO in inline assembly auf Variablen zugreifen
    __asm(
        "           \n\
          msr msp, r1 \n\
          bx r0       \n\
    ");
}

} // namespace bootloader
