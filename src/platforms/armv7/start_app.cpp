#include "platforms/start_app.h"

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

void Move_Vector_Table()
{
    constexpr uint32_t TBLOFF = 0xE000ED08;
    /* Set the vector table base address */
    uint32_t* vector_table = (uint32_t*)&__approm_start__; // NOLINT
    uint32_t* vtor = (uint32_t*)TBLOFF; // NOLINT
    *vtor = ((uint32_t)vector_table & 0xFFFFFFF8); // NOLINT
}

} // namespace bootloader
