#pragma once
#include "clkgen_setup.h"

namespace RODOS {

class XTALInitializer {
public:
    static void init(const XTALSetup& setup);
};

class PLLInitializer {
public:
    static void init(const PLLSetup& setup);
private:
    static void setClockSource(const PLLSetup& setup);
    static void disablePowerDown();
    static void setupValues(const PLLSetup& setup);
    static void enableReset();
    static void disableReset();
    static void reset();
};

class SYSCLKInitializer {
public:
    static void resetClockSource();
    static void init(const SYSCLKSetup& setup);
};

class ClockInitializer {
public:
    static void init(const ClockSetup& setup);
};

void shortDelay();

}
