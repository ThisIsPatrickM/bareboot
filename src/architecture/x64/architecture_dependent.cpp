#include "architecture/architecture_dependent.h"

#include "bootloader.h"

#include <iostream>

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
    std::cout << "Dummy App called. x64 Example ends here." << std::endl;
    while (true) {
        ;
    }
}

void* __bootrom_start__ = (&dummyBootloader); // NOLINT
int __bootrom_size__ = sizeof(dummyBootloader); // NOLINT
void* __approm_start__ = (void*)Dummy_App; // NOLINT
int __approm_size__ = 0; // NOLINT

namespace bootloader {

extern "C" [[noreturn, gnu::used]] void Reset_Handler();

void Start_App([[maybe_unused]] void* programmCounter, [[maybe_unused]] void* stackPointer)
{
    // TODO This only calls a Dummy_App.
    __asm__("call *%0" ::"m"(__approm_start__) :); // good
}

} // namespace bootloader

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) // NOLINT
{
    bootloader::Reset_Handler();
    return 0;
}