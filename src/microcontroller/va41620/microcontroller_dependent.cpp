#include "microcontroller/microcontroller_dependent.h"

/* Controller specific includes */
#include "architecture/architecture_dependent.h"
#include "microcontroller/va41620/hw_hal/hw_hal_watchdog.h"
#include "microcontroller/va41620/irq_handlers.h"
#include "platform_parameters.h"

namespace bootloader {

using namespace bootloader::va41620;
using namespace bootloader::architecture;

/*******************************************************************************
 * Global Functions for that every Microcontroller needs to implement
 ******************************************************************************/
void Move_Vector_Table()
{
    constexpr uint32_t TBLOFF = 0xE000ED08;
    /* Set the vector table base address */
    uint32_t* vector_table = (uint32_t*)&__approm_start__; // NOLINT
    auto* vtor = reinterpret_cast<uint32_t*>(TBLOFF); // NOLINT
    *vtor = ((uintptr_t)vector_table & 0xFFFFFFF8); // NOLINT
}

void Init_Watchdog()
{
    RODOS::HwHalWatchdog::hwInitWatchdog(WATCHDOG_TIMEOUT_MS);
}

void Trigger_Watchdog()
{
    RODOS::HwHalWatchdog::hwTriggerWatchdog();
}

/*******************************************************************************
 * Global Functions for Reset Handler
 ******************************************************************************/
typedef void (*constructor)(); // NOLINT
extern "C" constructor __init_array_start[]; // NOLINT
extern "C" constructor __init_array_end[]; // NOLINT

void Call_Constructors()
{
    for (constructor* i = __init_array_start; i != __init_array_end; i++) { // NOLINT
        (*i)();
    }
}

void Init_Data_Section()
{
    volatile uint32_t* destPtr = _sdata;
    const volatile uint32_t* srcPtr = _sidata;
    while (reinterpret_cast<uintptr_t>(destPtr) < reinterpret_cast<uintptr_t>(_edata)) { // NOLINT
        *destPtr = *srcPtr;
        destPtr++; // NOLINT
        srcPtr++; // NOLINT
    }
}

void Init_Bss_Section()
{
    volatile uint32_t* destPtr = _sbss;
    while (reinterpret_cast<uintptr_t>(destPtr) < reinterpret_cast<uintptr_t>(_ebss)) { // NOLINT
        *destPtr = 0;
        destPtr++; // NOLINT
    }
}

void Wait_About_5_Seconds()
{
    constexpr int32_t seconds { 5 }; // NOLINT
    constexpr int32_t frequencyHz { 20'000'000 }; // NOLINT
    constexpr int32_t approximateCyclesPerLoop { 3 }; // NOLINT
    constexpr int32_t totalLoops { (frequencyHz / approximateCyclesPerLoop) * seconds }; // NOLINT

    for (int32_t i { 0 }; i < totalLoops; i++) {
        asm volatile("");
    }
}

/*******************************************************************************
 * Entry Point from Linker
 ******************************************************************************/

extern "C" [[noreturn, gnu::used]] void Reset_Handler()
{
    Init_Watchdog();
    // TODO remove before flight
    Wait_About_5_Seconds();

    Memory_Barrier();

    Init_Data_Section();
    Init_Bss_Section();

    Memory_Barrier();
    Call_Constructors();
    Memory_Barrier();

    Trigger_Watchdog();

    Bootloader boot {};
    boot.run();

    while (true) {
        ;
    }
}

} // namespace bootloader
