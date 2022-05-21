#include "../../start_app.h"

// TODO this only fixes compiler/linker warnings, because I couldn't find a
// proper linkerscript for linux
uint32_t _sdata[1] = {}; // NOLINT
uint32_t _sidata[1] = {}; // NOLINT
uint32_t _sbss[1] = {}; // NOLINT
uint32_t _ebss[1] = {}; // NOLINT
uint32_t _estack[1] = {}; // NOLINT

extern "C" [[noreturn, gnu::used]] void Reset_Handler();

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) // NOLINT
{
    Reset_Handler();
    return 0;
}

namespace bootloader {

extern "C" void Start_App(
    [[maybe_unused]] void* programmCounter, [[maybe_unused]] void* stackPointer)
{
    // TODO in inline assembly auf Variablen zugreifen
    // TODO Assembly for Linux
}

} // namespace bootloader
