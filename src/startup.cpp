#include "startup.h"

namespace bootloader {

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

inline void Memory_Barrier()
{
    asm volatile("" : : : "memory");
}

void Init_Spi()
{
    RODOS::HAL_SPI spi { RODOS::SPI_IDX3 };
    spi.init();
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

extern "C" [[noreturn, gnu::used]] void Reset_Handler()
{
    // TODO remove before flight
    Wait_About_5_Seconds();
    // Memory_Barrier();
    Init_Data_Section();
    Init_Bss_Section();
    Memory_Barrier();
    // // setupFPU();
    // Memory_Barrier();
    Call_Constructors();
    Memory_Barrier();
    // // ClockInitializer::init(globalClockSetup);
    // Memory_Barrier();

    auto* appVectors = (DeviceVectors*)&__approm_start__; // NOLINT
    // TODO Set TBLOFF
    Start_App(appVectors->pfnResetHandler, appVectors->pvStack);

    while (true) {
        ;
    }
}

}
