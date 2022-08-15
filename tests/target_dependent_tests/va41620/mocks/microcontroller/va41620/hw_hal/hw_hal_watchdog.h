#pragma once

#include <cstdint>

namespace RODOS {

// NOLINTNEXTLINE(readability-identifier-naming)
class HwHalWatchdog {
public:
    static void hwInitWatchdog([[maybe_unused]] int32_t intervalMilliseconds) {}
    static void hwTriggerWatchdog() {}
};

} // namespace RODOS
