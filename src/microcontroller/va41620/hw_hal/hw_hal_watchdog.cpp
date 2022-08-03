#include "hw_hal_watchdog.h"

namespace RODOS {

void HW_HAL_WATCHDOG::hwInitWatchdog(int32_t intervalMilliseconds)
{
    if (intervalMilliseconds <= 0) {
        return;
    }
    // Enable clock
    SYSCONFIG->PERIPHERAL_CLK_ENABLE.write(SYSCONFIG_PERIPHERAL_CLK_ENABLE::Watchdog(1U));

    // Load timeout value
    uint32_t timeoutValue = convertMillisecondsToCycles(intervalMilliseconds);
    WATCHDOG->WDOGLOAD.write(timeoutValue);

    // Enable Reset
    WATCHDOG->WDOGCONTROL.write(WD_WDOGCONTROL::RESEN(1U));
}

void HW_HAL_WATCHDOG::hwTriggerWatchdog()
{
    WATCHDOG->WDOGINTCLR.write(1U);
}

constexpr uint32_t HW_HAL_WATCHDOG::convertMillisecondsToCycles(int32_t milliseconds)
{
    if (static_cast<uint32_t>(milliseconds) >= MAX_TIMEOUT_MILLISECONDS) {
        return UINT32_MAX;
    }
    return SYS_CLOCK_FREQUENCY * (static_cast<uint32_t>(milliseconds));
}

} // namespace RODOS
