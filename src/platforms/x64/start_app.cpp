#include "../../start_app.h"
#include "bootloader.h"

using namespace bootloader;

// TODO this only fixes compiler/linker warnings, because I couldn't find a
// proper linkerscript for linux
volatile uint32_t _sdata[1] = {}; // NOLINT
volatile uint32_t _sidata[1] = {}; // NOLINT
volatile uint32_t _sbss[1] = {}; // NOLINT
volatile uint32_t _ebss[1] = {}; // NOLINT
volatile uint32_t _estack[1] = {}; // NOLINT

Bootloader dummyBootloader;
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

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) // NOLINT
{
    bootloader::Reset_Handler();
    return 0;
}

namespace bootloader {

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

} // namespace bootloader
