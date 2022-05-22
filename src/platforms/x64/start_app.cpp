#include "platforms/start_app.h"
#include "bootloader.h"

// TODO this only fixes compiler/linker warnings, because I couldn't find a
// proper linkerscript for linux
volatile uint32_t _sdata[1] = {}; // NOLINT
volatile uint32_t _sidata[1] = {}; // NOLINT
volatile uint32_t _sbss[1] = {}; // NOLINT
volatile uint32_t _ebss[1] = {}; // NOLINT
volatile uint32_t _estack[1] = {}; // NOLINT

bootloader::Bootloader dummyBootloader;
void Dummy_App()
{
    while (true) {
        ;
    }
}

void* __bootrom_start__ = &dummyBootloader; // NOLINT
int __bootrom_size__ = sizeof(dummyBootloader); // NOLINT
void* __approm_start__ = (void*)Dummy_App; // NOLINT
int __approm_size__ = 0; // NOLINT

namespace bootloader {

extern "C" [[noreturn, gnu::used]] void Reset_Handler();

void Start_App([[maybe_unused]] void* programmCounter, [[maybe_unused]] void* stackPointer)
{
    // TODO in inline assembly auf Variablen zugreifen
    // TODO Assembly for Linux
    // TODO Clear stack
    __asm(
        "push %0;"
        "ret"
        : // Output Operands
        : "r"(programmCounter) // Input operands
    );
}

void Move_Vector_Table() {}

} // namespace bootloader

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) // NOLINT
{
    bootloader::Reset_Handler();
    return 0;
}