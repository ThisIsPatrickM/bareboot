#include "architecture/architecture_dependent.h"

namespace bootloader {

struct DeviceVectors {
    /* Stack pointer */
    void* pvStack;
    /* Cortex-M handlers */
    void* pfnResetHandler;
};

void Start_App()
{
    auto* appVectors = reinterpret_cast<DeviceVectors*>(&__approm_start__); // NOLINT
    // Start_App(appVectors->pfnResetHandler, appVectors->pvStack);
    // Start_App(appVectors->pfnResetHandler,
    //           (void*)(_estack)); // TODO This might cause errors
    __asm(
        "msr msp, %0;"
        "bx %1;"
        : // Output Operands
        // : "r"(stackPointer), "r"(programmCounter) // Input operands
        // : "r"(appVectors->pvStack), "r"(appVectors->pfnResetHandler) // Input operands
        : "r"((void*)(_estack)), "r"(appVectors->pfnResetHandler) // Input operands
    );
}

} // namespace bootloader
