#include "hw_hal_watchdog.h"

namespace RODOS {

constexpr uint32_t HwHalWatchdog::convertMillisecondsToCycles(int32_t milliseconds)
{
    if (static_cast<uint32_t>(milliseconds) >= MAX_TIMEOUT_MILLISECONDS) {
        return UINT32_MAX;
    }
    return GLOBAL_CLOCK_SETUP.getAPB2Frequency() * (static_cast<uint32_t>(milliseconds));
}

void HwHalWatchdog::hwInitWatchdog(int32_t intervalMilliseconds)
{
    if (intervalMilliseconds <= 0) {
        return;
    }

    SYSCONFIG_PERIPHERAL_BITBAND->WATCHDOG_RESET.write(0);
    SYSCONFIG_PERIPHERAL_BITBAND->WATCHDOG_RESET.write(1);
    SYSCONFIG_PERIPHERAL_BITBAND->WATCHDOG_ENABLE.write(1U);

    // Enable Reset
    WATCHDOG->WDOGCONTROL.write(WD_WDOGCONTROL::RESEN(1U), WD_WDOGCONTROL::INTEN(1U));
    WATCHDOG->WDOGLOAD.write(convertMillisecondsToCycles(intervalMilliseconds));
}

void HwHalWatchdog::hwTriggerWatchdog()
{
    WATCHDOG->WDOGLOAD.write(WATCHDOG->WDOGLOAD.read());
}

} // namespace RODOS
