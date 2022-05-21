#pragma once
#include "clkgen_config.h"
#include "platform-parameter.h"

namespace RODOS {

class SysTickManager{
private:
    static constexpr int32_t freq { globalClockSetup.getSysclkFrequency() };

    static constinit uint32_t ticks;

    static inline void setReloadValue(uint32_t ticks);
    static inline void resetCurrentVal();
    static inline void enable();
public:
    static void start();
    static void setInterval(int64_t microseconds);
    static void stop();

    static constexpr uint32_t maxTicks = 0x00FF'FFFF;
    static constexpr int64_t calculateTicks(int64_t microseconds){
        return (freq * microseconds) / 1'000'000;
    }

};


}
