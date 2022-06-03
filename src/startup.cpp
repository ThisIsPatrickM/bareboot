#include "startup.h"

namespace bootloader {

constinit const GlobalImageMetadata GLOBAL_IMAGE_METADATA
    __attribute__((used, section(".metadata_table")));

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

    Toggle_ROM_Writeable(true);
    Memory_Barrier();

    Init_Data_Section();
    Init_Bss_Section();

    Memory_Barrier();
    Call_Constructors();
    Memory_Barrier();
    // TODO Block ROM later agian
    // Toggle_ROM_Writeable(false);
    Memory_Barrier();
    // Memroy Barrier is important!
    Move_Vector_Table();
    Memory_Barrier();
    Start_App();

    while (true) {
        ;
    }
}

}
