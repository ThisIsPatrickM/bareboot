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

// extern "C" void* __dso_handle;
// void*            __dso_handle = 0;
// extern "C" {
// int  __cxa_atexit(void (*f)(void*), void* p, void* d);
// void __cxa_pure_virtual(void);
// int  __cxa_guard_acquire(unsigned char* g);
// void __cxa_guard_release(unsigned char* g);
// }
// int __cxa_atexit([[maybe_unused]] void (*f)(void*), [[maybe_unused]] void* p,
// [[maybe_unused]] void* d) {
//     return 0;
// }

// // Errorhandler, der ausgefuehrt wird, wenn eine "pure virtual"
// // Funktion aufgerufen wird
// void __cxa_pure_virtual(void) {
//     while(1) {}
// }

// int __cxa_guard_acquire(unsigned char* g) {
//     if(*g == 0) {
//         *g = 1;
//         return 1; // success
//     } else {
//         return 0;
//     }
// }

// void __cxa_guard_release(unsigned char* g) {
//     *g = 2;
// }
} // namespace bootloader
