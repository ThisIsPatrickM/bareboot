#include "peripheral_ctrl/clock_management/busywait_hbo.h"
#include "peripheral_ctrl/clock_management/clkgen_setup.h"

namespace RODOS {
void waitMicrosecHBO(uint32_t microseconds){
    constexpr uint32_t cyclesPerLoop { 3 };
    uint32_t loops = microseconds*(HBO::frequency/1'000'000/cyclesPerLoop);
    for(uint32_t i { 0 }; i<loops;i++){
        asm volatile("");
    }
}
}
