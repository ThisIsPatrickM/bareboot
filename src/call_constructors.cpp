#include "call_constructors.h"

typedef void (*constructor)(); // NOLINT

// Im Linkerskript definiert
namespace bootloader {
extern "C" constructor __init_array_start[]; // NOLINT
extern "C" constructor __init_array_end[]; // NOLINT

void Call_Constructors()
{
    for (constructor* i = __init_array_start; i != __init_array_end; i++) { // NOLINT
        (*i)();
    }
}

} // namespace bootloader
