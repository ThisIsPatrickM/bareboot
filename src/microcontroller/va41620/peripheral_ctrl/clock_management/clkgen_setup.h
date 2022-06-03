#pragma once

#include "clkgen_configstruct.h"

namespace rodos {

struct HBO {
    static constexpr int32_t FREQUENCY { 20'000'000 };
};

class XTALSetup {
public:
    template <XtalMode MODE, int32_t FREQUENCY>
    static constexpr XTALSetup generateSetup();

    [[nodiscard]] constexpr XtalMode getMode() const { return mode; }
    [[nodiscard]] constexpr int32_t getFrequency() const { return frequency; }

    const XtalMode mode;
    const int32_t frequency;

private:
    constexpr XTALSetup(XtalMode mode, int32_t frequency)
        : mode { mode }
        , frequency { frequency }
    {
    }
};

template <XtalMode MODE, int32_t FREQUENCY>
constexpr XTALSetup XTALSetup::generateSetup()
{
    if constexpr (MODE == XtalMode::EXT_CLK) {
        static_assert(FREQUENCY <= 100'000'000); // NOLINT
    } else if constexpr (MODE == XtalMode::XTAL) {
        static_assert(FREQUENCY >= 4'000'000 && FREQUENCY <= 10'000'000); // NOLINT
    }
    return { MODE, FREQUENCY };
}

// NOLINTBEGIN
/**
 * specifies a setup of the PLL
 * all parameter calculations are equivalent to those from Vorago's hal_clkgen.c
 */
class PLLSetup {
public:
    template <
        bool enable,
        const XTALSetup xtalSetup,
        int32_t targetFrequency,
        int32_t requireExactFrequency>
    static constexpr PLLSetup generateSetup();

    constexpr PllSource getSource() const { return m_source; }
    constexpr int32_t getFrequency() const { return m_frequency; }

    constexpr uint32_t getBWADJRegValue() const { return static_cast<uint32_t>(m_NB - 1); }
    constexpr uint32_t getCLKRRegValue() const { return static_cast<uint32_t>(m_NR - 1); }
    constexpr uint32_t getCLKFRegValue() const { return static_cast<uint32_t>(m_NF - 1); };
    constexpr uint32_t getCLKODRegValue() const { return static_cast<uint32_t>(m_OD - 1); }

    PllSource m_source;
    int32_t m_frequency;
    int32_t m_NR;
    int32_t m_OD;
    int32_t m_NF;
    int32_t m_NB;

    int32_t m_vcoInputFreq;
    int32_t m_vcoOutputFreq;

private:
    static constexpr int32_t maxNR { 16 };
    static constexpr int32_t maxNF { 64 };
    static constexpr int32_t calculateNR(int32_t refClockFrequency);
    static constexpr int32_t calculateOD(int32_t vcoInputFrequency, int32_t targetFrequency);
    static constexpr int32_t calculateNF(
        int32_t vcoInputFrequency, int32_t OD, int32_t targetFrequency);
    static constexpr PllSource determinePLLSource(const XTALSetup& xtalSetup);

    constexpr PLLSetup(
        PllSource source,
        int32_t frequency,
        int32_t NR,
        int32_t OD,
        int32_t NF,
        int32_t NB,
        int32_t vcoInputFreq,
        int32_t vcoOutputFreq)
        : m_source { source }
        , m_frequency { frequency }
        , m_NR { NR }
        , m_OD { OD }
        , m_NF { NF }
        , m_NB { NB }
        , m_vcoInputFreq { vcoInputFreq }
        , m_vcoOutputFreq { vcoOutputFreq }
    {
    }
};

constexpr int32_t PLLSetup::calculateNR(int32_t inputFrequency)
{
    constexpr int32_t targetedVcoInputFreq { 5'000'000 };
    int32_t result = inputFrequency / targetedVcoInputFreq;
    if (result == 0) {
        result = 1;
    } else if (result > maxNR) {
        result = maxNR;
    }
    return result;
}

constexpr int32_t PLLSetup::calculateOD(int32_t vcoInputFrequency, int32_t targetFrequency)
{
    int32_t result { 5 };
    int32_t mulRatio { targetFrequency / vcoInputFrequency };
    while (mulRatio * result > maxNF) {
        result--;
    }
    if (result == 0) {
        result = 1;
    }
    return result;
}

constexpr PllSource PLLSetup::determinePLLSource(const XTALSetup& xtalSetup)
{
    switch (xtalSetup.getMode()) {
    case XtalMode::OFF:
        return PllSource::NONE;
    case XtalMode::XTAL:
        return PllSource::XTAL;
    case XtalMode::EXT_CLK:
        return PllSource::EXT_CLK;
    }
}

constexpr int32_t PLLSetup::calculateNF(
    int32_t vcoInputFrequency, int32_t OD, int32_t targetFrequency)
{
    int32_t result { (targetFrequency / vcoInputFrequency) * OD };
    if (result == 0) {
        result = 1;
    } else if (result > maxNF) {
        result = maxNF;
    }
    return result;
}

template <
    bool enable,
    const XTALSetup xtalSetup,
    int32_t targetFrequency,
    int32_t requireExactFrequency>
constexpr PLLSetup PLLSetup::generateSetup()
{
    if constexpr (enable) {
        static_assert(xtalSetup.getMode() != XtalMode::OFF, "XTAL must be used to use PLL");
        static_assert(targetFrequency > 0);
        static_assert(targetFrequency <= 100'000'000);
        constexpr int32_t refClockFrequency { xtalSetup.getFrequency() };
        constexpr int32_t NR { calculateNR(refClockFrequency) };

        constexpr int32_t vcoInputFrequency { refClockFrequency / NR };
        static_assert(4'000'000 <= vcoInputFrequency && vcoInputFrequency <= 10'000'000);

        constexpr int32_t OD { calculateOD(vcoInputFrequency, targetFrequency) };
        static_assert(1 <= OD && OD <= 0x10);
        constexpr int32_t NF { calculateNF(vcoInputFrequency, OD, targetFrequency) };
        constexpr int32_t NB { NF };
        static_assert(1 <= NB && NB <= 0x40);
        constexpr int32_t vcoOutputFrequency { vcoInputFrequency * NF };
        constexpr int32_t frequency { vcoOutputFrequency / OD };
        static_assert(frequency <= 100'000'000);
        if constexpr (requireExactFrequency) {
            static_assert(frequency == targetFrequency);
        }
        constexpr PllSource source = determinePLLSource(xtalSetup);
        return { source, frequency, NR, OD, NF, NB, vcoInputFrequency, vcoOutputFrequency };
    } else {
        return { PllSource::NONE, 0, 0, 0, 0, 0, 0, 0 };
    }
}

struct SYSCLKSetup {
public:
    template <SysclkSource source, const XTALSetup xtalSetup, const PLLSetup PLLSetup>
    static constexpr SYSCLKSetup generateSetup();

    constexpr SysclkSource getSource() const { return m_source; }
    constexpr int32_t getFrequency() const { return m_frequency; }
    constexpr int32_t getAPB1Frequency() const { return m_frequency / 2; }
    constexpr int32_t getAPB2Frequency() const { return m_frequency / 4; }

    SysclkSource m_source;
    int32_t m_frequency;

private:
    static constexpr int32_t determineFrequency(
        SysclkSource source, const XTALSetup& xtalSetup, const PLLSetup& pllSetup);
    constexpr SYSCLKSetup(SysclkSource source, int32_t frequency)
        : m_source { source }
        , m_frequency { frequency }
    {
    }
};

constexpr int32_t SYSCLKSetup::determineFrequency(
    SysclkSource source, const XTALSetup& xtalSetup, const PLLSetup& pllSetup)
{
    switch (source) {
    case SysclkSource::HBO:
        return HBO::FREQUENCY;
    case SysclkSource::EXT_CLK:
        return xtalSetup.getFrequency();
    case SysclkSource::PLL:
        return pllSetup.getFrequency();
    case SysclkSource::XTAL:
        return xtalSetup.getFrequency();
    }
}

template <SysclkSource source, const XTALSetup xtalSetup, const PLLSetup pllSetup>
constexpr SYSCLKSetup SYSCLKSetup::generateSetup()
{
    constexpr int32_t frequency { determineFrequency(source, xtalSetup, pllSetup) };
    static_assert(frequency > 0);
    static_assert(frequency <= 100'000'000);
    return { source, frequency };
}

class ClockSetup {
public:
    template <const ClkgenConfigStruct config>
    static constexpr ClockSetup generateSetup();

    constexpr const PLLSetup& getPllSetup() const { return m_pllSetup; }
    constexpr const XTALSetup& getXtalSetup() const { return m_xtalSetup; }
    constexpr const SYSCLKSetup& getSysclkSetup() const { return m_sysclkSetup; }
    constexpr int32_t getSysclkFrequency() const { return m_sysclkSetup.getFrequency(); }
    constexpr int32_t getAPB1Frequency() const { return m_sysclkSetup.getAPB1Frequency(); }
    constexpr int32_t getAPB2Frequency() const { return m_sysclkSetup.getAPB2Frequency(); }

private:
    const XTALSetup m_xtalSetup;
    const PLLSetup m_pllSetup;
    const SYSCLKSetup m_sysclkSetup;

    constexpr ClockSetup(XTALSetup xtalSetup, PLLSetup pllSetup, SYSCLKSetup sysclkSetup)
        : m_xtalSetup { xtalSetup }
        , m_pllSetup { pllSetup }
        , m_sysclkSetup { sysclkSetup }
    {
    }
};

template <const ClkgenConfigStruct config>
constexpr ClockSetup ClockSetup::generateSetup()
{
    constexpr XTALSetup xtalSetup { XTALSetup::generateSetup<config.xtalMode, config.xtalFreq>() };
    constexpr PLLSetup pllSetup { PLLSetup::generateSetup<
        config.usePLL,
        xtalSetup,
        config.pllTargetFrequency,
        config.requireExactPLLFrequency>() };
    constexpr SYSCLKSetup sysclkSetup {
        SYSCLKSetup::generateSetup<config.sysclkSource, xtalSetup, pllSetup>()
    };
    return { xtalSetup, pllSetup, sysclkSetup };
}
// NOLINTEND
}
