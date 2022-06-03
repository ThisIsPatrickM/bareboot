#include "microcontroller/microcontroller_dependent.h"

namespace bootloader {

void Move_Vector_Table()
{
    constexpr uint32_t TBLOFF = 0xE000ED08;
    /* Set the vector table base address */
    /** THis worked!*/
    uint32_t* vector_table = (uint32_t*)&__approm_start__; // NOLINT
    auto* vtor = reinterpret_cast<uint32_t*>(TBLOFF); // NOLINT
    *vtor = ((uintptr_t)vector_table & 0xFFFFFFF8); // NOLINT
    // TODO If anything fails, try this:
    // *vtor = ((uint32_t)vector_table & 0xFFFFFFF8); // NOLINT
}

void Toggle_ROM_Writeable(bool writeable)
{
    constexpr uint32_t ROM_PROT_ADDRESS = 0x40010010;
    uint32_t* ROM_PROT = reinterpret_cast<uint32_t*>(ROM_PROT_ADDRESS); // NOLINT
    if (writeable) {
        *ROM_PROT = 0x1;
    } else {
        *ROM_PROT = 0x0;
    }
}

} // namespace bootloader
