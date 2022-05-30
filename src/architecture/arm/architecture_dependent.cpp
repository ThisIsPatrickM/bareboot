#include "architecture/architecture_dependent.h"

namespace bootloader {

void Start_App(void* programmCounter, void* stackPointer)
{
    __asm(
        "msr msp, %0;"
        "bx %1;"
        : // Output Operands
        : "r"(stackPointer), "r"(programmCounter) // Input operands
    );
}

} // namespace bootloader
