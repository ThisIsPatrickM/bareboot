#include "architecture/architecture_dependent.h"

#include "metadata.h"

namespace bootloader::architecture {

// extern "C" [[noreturn, gnu::used]] void Reset_Handler();

void Start_App()
{
    // TODO This only calls a Dummy_App.
    __asm__("call *%0" ::"m"(__approm_start__) :);
}

void Memory_Barrier()
{
    // TODO: This was only copied from previous code and needs to be checked, if it really is a
    // memory barrier
    // __asm__ volatile("" : : : "memory");
    asm volatile("mfence" ::: "memory");
}

} // namespace bootloader::architecture
