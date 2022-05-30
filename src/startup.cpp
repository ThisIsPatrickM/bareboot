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
    //     SPI ROM boot port (ROM_SCK, ROM_SS, ROM_MISO, and
    // ROM_MOSI).

    // RODOS::GPIO_PIN sckPin;
    // RODOS::GPIO_PIN misoPin;
    // RODOS::GPIO_PIN scmosiPinkPin;
    // RODOS::GPIO_PIN nssPin;

    // RODOS::HAL_SPI spi { RODOS::SPI_IDX3, sckPin, misoPin, scmosiPinkPin, nssPin };
    // uint32_t buffer[20] { 0 };
    // spi.init();
    // spi.read(buffer, sizeof(buffer));
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
    // Wait_About_5_Seconds();

    Memory_Barrier();

    // Remove ROM_PROT
    Toggle_ROM_Writeable(true);

    Memory_Barrier();
    Init_Spi();

    // auto* bl = (Bootloader*)&__bootrom_start__; // NOLINT
    Init_Data_Section();
    Init_Bss_Section();
    Memory_Barrier();
    Call_Constructors();
    Memory_Barrier();

    Toggle_ROM_Writeable(false);
    Memory_Barrier();
    // Memroy Barrier is important!
    Move_Vector_Table();
    Memory_Barrier();

    auto* appVectors = reinterpret_cast<DeviceVectors*>(&__approm_start__); // NOLINT
    // Start_App(appVectors->pfnResetHandler, appVectors->pvStack);
    Start_App(appVectors->pfnResetHandler,
              (void*)(_estack)); // TODO This might cause errors

    while (true) {
        ;
    }
}

}
