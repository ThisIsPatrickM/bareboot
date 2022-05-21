#include "clock_initializers.h"
#include "peripheral_ctrl/clock_management/busywait_hbo.h"
#include "peripheral_ctrl/peripheral_defs.h"
#include "peripheral_ctrl/register_maps/clkgen_registers.h"

namespace RODOS {

void shortDelay()
{
    waitMicrosecHBO(5'000);
}

void XTALInitializer::init(const XTALSetup& setup)
{
    using namespace CLKGEN_CTRL1;
    clkgen->CTRL1.set(
        XTAL_EN(setup.getMode() == XTAL_MODE::XTAL ? 1 : 0),
        XTAL_N_EN(setup.getMode() == XTAL_MODE::EXT_CLK ? 1 : 0));
    if (setup.getMode() == XTAL_MODE::XTAL) {
        waitMicrosecHBO(15'000);
    } else {
        shortDelay();
    }
}

void PLLInitializer::setClockSource(const PLLSetup& setup)
{
    using namespace CLKGEN_CTRL0;
    clkgen->CTRL0.set(REF_CL_SEL(uint32_t(setup.getSource())));
}
void PLLInitializer::disablePowerDown()
{
    using namespace CLKGEN_CTRL0;
    clkgen->CTRL0.set(PLL_PWDN(0));
    shortDelay();
}
void PLLInitializer::setupValues(const PLLSetup& setup)
{
    using namespace CLKGEN_CTRL0;
    clkgen->CTRL0.set(
        PLL_BWADJ(setup.getBWADJRegValue()),
        PLL_CLKOD(setup.getCLKODRegValue()),
        PLL_CLKF(setup.getCLKFRegValue()),
        PLL_CLKR(setup.getCLKRRegValue()));

    clkgen->CTRL0.set(PLL_TEST(0), PLL_BYPASS(0), PLL_INTFB(1));
    shortDelay();
}
void PLLInitializer::enableReset()
{
    using namespace CLKGEN_CTRL0;
    clkgen->CTRL0.set(PLL_RESET(1));
    shortDelay();
}
void PLLInitializer::disableReset()
{
    using namespace CLKGEN_CTRL0;
    clkgen->CTRL0.set(PLL_RESET(0));
    shortDelay();
}
void PLLInitializer::reset()
{
    enableReset();
    disableReset();
}

void PLLInitializer::init(const PLLSetup& setup)
{
    if (setup.getSource() != PLL_SOURCE::NONE) {
        setClockSource(setup);
        disablePowerDown();
        setupValues(setup);
        reset();
    }
}

void SYSCLKInitializer::resetClockSource()
{
    using namespace CLKGEN_CTRL0;
    clkgen->CTRL0.set(CLKSEL_SYS(0));
    shortDelay();
}
void SYSCLKInitializer::init(const SYSCLKSetup& setup)
{
    using namespace CLKGEN_CTRL0;
    clkgen->CTRL0.set(CLKSEL_SYS(static_cast<uint32_t>(setup.getSource())));
}

void ClockInitializer::init(const ClockSetup& setup)
{
    sysconfigPeripheralBitband->CLKGEN_ENABLE.write(uint32_t(1));
    SYSCLKInitializer::resetClockSource();
    XTALInitializer::init(setup.getXtalSetup());
    PLLInitializer::init(setup.getPllSetup());
    SYSCLKInitializer::init(setup.getSysclkSetup());
}

}
