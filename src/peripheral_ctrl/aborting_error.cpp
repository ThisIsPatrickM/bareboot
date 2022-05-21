#include "src/peripheral_ctrl/aborting_error.h"

namespace RODOS {

[[noreturn]] void abortingError(const char* str)
{
    // RODOS_ERROR(str);
    asm("bkpt");
    __builtin_trap();
}

}
