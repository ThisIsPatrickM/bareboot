#pragma once

#include "microcontroller/va41620/rodos_includes/peripheral_ctrl/peripheral_defs.h"
#include "microcontroller/va41620/rodos_includes/subtargets/peb1_100mhz/clkgen_config.h"

#include <cstdint>

namespace RODOS {

// NOLINTNEXTLINE(readability-identifier-naming)
class HwHalWatchdog {
public:
    static void hwInitWatchdog(int32_t intervalMilliseconds);
    static void hwTriggerWatchdog();

private:
    static constexpr uint64_t MAX_TIMEOUT_MILLISECONDS =
        (UINT32_MAX / GLOBAL_CLOCK_SETUP.getAPB2Frequency());

    static constexpr uint32_t convertMillisecondsToCycles(int32_t milliseconds);
};

} // namespace RODOS
