#include "architecture/architecture_dependent.h"

namespace bootloader::architecture {

struct AppVectorTable {
    void* stackPointer;
    void* resetHandler;
};

void Start_App()
{
    auto* appVectors = reinterpret_cast<AppVectorTable*>(&__approm_start__); // NOLINT

    // Start Application by setting stackpointer and branch to resetHandler
    __asm(
        "msr msp, %0;"
        "bx %1;"
        : // Output Operands
        : "r"(appVectors->stackPointer), "r"(appVectors->resetHandler) // Input operands
    );
}

void Disable_Interrupts()
{
    asm volatile("cpsid i");
}

void Enable_Interrupts()
{
    asm volatile("cpsie i");
}

} // namespace bootloader::architecture
