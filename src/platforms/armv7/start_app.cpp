#include "../../start_app.h"

namespace bootloader {

void Start_App([[maybe_unused]] void* programmCounter, [[maybe_unused]] void* stackPointer)
{
    __asm(
        "msr msp, %0;"
        "bx %1;"
        : // Output Operands
        : "r"(stackPointer), "r"(programmCounter) // Input operands
    );
}

} // namespace bootloader
