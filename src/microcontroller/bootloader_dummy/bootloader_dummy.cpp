#include "metadata.h"

#include <iostream>

/* Dummy Implementations */
bootloader::GlobalImageMetadata dummyBootloader;
void Dummy_App()
{
    std::cout << "Dummy App called. Example ends here." << std::endl;
    while (true) {
        ;
    }
}

// TODO this only fixes compiler/linker warnings, because I couldn't find a
// proper linkerscript for linux
// volatile uint32_t _sdata[1] = {}; // NOLINT
// volatile uint32_t _sidata[1] = {}; // NOLINT
// volatile uint32_t _sbss[1] = {}; // NOLINT
// volatile uint32_t _ebss[1] = {}; // NOLINT
// volatile uint32_t _estack[1] = {}; // NOLINT

void* __approm_start__ = (void*)Dummy_App; // NOLINT
int __approm_size__ = 0; // NOLINT
void* __bootloader__ = &dummyBootloader; // NOLINT
