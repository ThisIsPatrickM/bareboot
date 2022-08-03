#pragma once

#include "microcontroller/va41620/peripheral_ctrl/peripheral_defs.h"

#include <cstdint>

namespace RODOS {

// NOLINTNEXTLINE(readability-identifier-naming)
class HW_HAL_WATCHDOG {
public:
    static void hwInitWatchdog(int32_t intervalMilliseconds);
    static void hwTriggerWatchdog();

private:
    static constexpr uint32_t FREQUENCY_HZ { 20'000'000 };
    static constexpr uint32_t FREQUENCY_PER_MILLISECOND = FREQUENCY_HZ / 1000U;
    static constexpr uint32_t SYS_CLOCK_FREQUENCY = FREQUENCY_PER_MILLISECOND / 4;
    static constexpr uint64_t MAX_TIMEOUT_MILLISECONDS = (UINT32_MAX / SYS_CLOCK_FREQUENCY);

    static constexpr uint32_t convertMillisecondsToCycles(int32_t milliseconds);
};

} // namespace RODOS
