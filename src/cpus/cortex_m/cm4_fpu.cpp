#include "cm4_fpu.h"
#include "peripheral_defs.h"

namespace RODOS {

extern "C" void setupFPU() {
    using namespace SCB_CPACR;
    scb.CPACR.write(CP10(3), CP11(3));
}

} // namespace RODOS
