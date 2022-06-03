#include "architecture/architecture_dependent.h"

namespace bootloader {

struct AppVectorTable {
    /* Stack pointer */
    void* stackPointer;
    /* Cortex-M handlers */
    void* resethandler;
};

void Start_App()
{
    auto* appVectors = reinterpret_cast<AppVectorTable*>(&__approm_start__); // NOLINT

    __asm(
        "msr msp, %0;"
        "bx %1;"
        : // Output Operands
        : "r"(appVectors->stackPointer), "r"(appVectors->resethandler) // Input operands
    );
}

void Memory_Barrier()
{
    __asm__ volatile("" : : : "memory");
}

} // namespace bootloader
