#pragma once
#include "clkgen_configstruct.h"

namespace RODOS {

struct HBO {
    static constexpr int32_t frequency { 20'000'000 };
};


class XTALSetup {
public:
    template<XTAL_MODE mode, int32_t frequency>
    static constexpr XTALSetup generateSetup();

    constexpr XTAL_MODE getMode() const{
        return m_mode;
    }
    constexpr int32_t getFrequency() const{
        return m_frequency;
    }

    const XTAL_MODE m_mode;
    const int32_t m_frequency;

private:
    constexpr XTALSetup(XTAL_MODE mode, int32_t frequency)
    : m_mode{mode}
    , m_frequency{frequency}
    {}
};

template<XTAL_MODE mode, int32_t frequency>
constexpr XTALSetup XTALSetup::generateSetup(){
    if constexpr (mode==XTAL_MODE::EXT_CLK){
        static_assert(frequency<=100'000'000);
    }else if constexpr (mode==XTAL_MODE::XTAL){
        static_assert(frequency>=4'000'000&&frequency<=10'000'000); 
    }
    return {mode, frequency};
}

/**
 * specifies a setup of the PLL
 * all parameter calculations are equivalent to those from Vorago's hal_clkgen.c
 */
class PLLSetup {
public:
    template<bool enable, const XTALSetup xtalSetup, int32_t targetFrequency, int32_t requireExactFrequency>
    static constexpr PLLSetup generateSetup();

    constexpr PLL_SOURCE getSource() const {
        return m_source;
    }
    constexpr int32_t getFrequency() const {
        return m_frequency;
    }

    constexpr uint32_t getBWADJRegValue() const {
        return static_cast<uint32_t>(m_NB-1);
    }
    constexpr uint32_t getCLKRRegValue() const {
        return static_cast<uint32_t>(m_NR-1);
    }
    constexpr uint32_t getCLKFRegValue() const {
        return static_cast<uint32_t>(m_NF-1);
    };
    constexpr uint32_t getCLKODRegValue() const {
        return static_cast<uint32_t>(m_OD-1);
    }

    PLL_SOURCE m_source;
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
    static constexpr int32_t calculateNF(int32_t vcoInputFrequency, int32_t OD, int32_t targetFrequency);
    static constexpr PLL_SOURCE determinePLLSource(const XTALSetup& xtalSetup);

    constexpr PLLSetup(PLL_SOURCE source, int32_t frequency, int32_t NR, int32_t OD, int32_t NF, int32_t NB, int32_t vcoInputFreq, int32_t vcoOutputFreq)
    : m_source{source}
    , m_frequency{frequency}
    , m_NR{NR}
    , m_OD{OD}
    , m_NF{NF}
    , m_NB{NB}
    , m_vcoInputFreq{vcoInputFreq}
    , m_vcoOutputFreq{vcoOutputFreq}
    {}
};

constexpr int32_t PLLSetup::calculateNR(int32_t inputFrequency){
    constexpr int32_t targetedVcoInputFreq { 5'000'000 };
    int32_t result = inputFrequency/targetedVcoInputFreq;
    if (result==0){
        result = 1;
    }else if (result>maxNR){
        result = maxNR;
    }
    return result;
}

constexpr int32_t PLLSetup::calculateOD(int32_t vcoInputFrequency, int32_t targetFrequency){
    int32_t result { 5 };
    int32_t mulRatio { targetFrequency/vcoInputFrequency };
    while(mulRatio*result > maxNF){
        result--;
    }
    if(result == 0){
        result = 1;
    }
    return result;
}

constexpr PLL_SOURCE PLLSetup::determinePLLSource(const XTALSetup& xtalSetup){
    switch(xtalSetup.getMode()){
    case XTAL_MODE::OFF:
        return PLL_SOURCE::NONE;
    case XTAL_MODE::XTAL:
        return PLL_SOURCE::XTAL;
    case XTAL_MODE::EXT_CLK:
        return PLL_SOURCE::EXT_CLK;
    }
}

constexpr int32_t PLLSetup::calculateNF(int32_t vcoInputFrequency, int32_t OD, int32_t targetFrequency){
    int32_t result { (targetFrequency/vcoInputFrequency)*OD };
    if(result == 0){
        result = 1;
    }else if(result>maxNF){
        result = maxNF;
    }
    return result;
}


template<bool enable, const XTALSetup xtalSetup, int32_t targetFrequency, int32_t requireExactFrequency>
constexpr PLLSetup PLLSetup::generateSetup(){
    if constexpr(enable){
        static_assert(xtalSetup.getMode()!=XTAL_MODE::OFF, "XTAL must be used to use PLL");
        static_assert(targetFrequency>0);
        static_assert(targetFrequency<=100'000'000);
        constexpr int32_t refClockFrequency { xtalSetup.getFrequency() };
        constexpr int32_t NR { calculateNR(refClockFrequency) };

        constexpr int32_t vcoInputFrequency{ refClockFrequency/NR };
        static_assert(4'000'000<=vcoInputFrequency && vcoInputFrequency<=10'000'000);

        constexpr int32_t OD { calculateOD(vcoInputFrequency, targetFrequency) };
        static_assert(1<=OD && OD<=0x10);
        constexpr int32_t NF { calculateNF(vcoInputFrequency, OD, targetFrequency) };
        constexpr int32_t NB { NF };
        static_assert(1<=NB && NB<=0x40);
        constexpr int32_t vcoOutputFrequency { vcoInputFrequency * NF };
        constexpr int32_t frequency { vcoOutputFrequency/OD };
        static_assert(frequency<=100'000'000);
        if constexpr (requireExactFrequency){
            static_assert(frequency == targetFrequency);
        }
        constexpr PLL_SOURCE source = determinePLLSource(xtalSetup);
        return {source, frequency, NR, OD, NF, NB, vcoInputFrequency, vcoOutputFrequency};
    }else{
        return {PLL_SOURCE::NONE, 0, 0, 0, 0, 0, 0, 0};
    }
}

struct SYSCLKSetup{
public:
    template<SYSCLK_SOURCE source, const XTALSetup xtalSetup, const PLLSetup PLLSetup>
    static constexpr SYSCLKSetup generateSetup();

    constexpr SYSCLK_SOURCE getSource() const {
        return m_source;
    }
    constexpr int32_t getFrequency() const {
        return m_frequency;
    }
    constexpr int32_t getAPB1Frequency() const {
        return m_frequency/2;
    }
    constexpr int32_t getAPB2Frequency() const {
        return m_frequency/4;
    }

    SYSCLK_SOURCE m_source;
    int32_t m_frequency;

private:
    static constexpr int32_t determineFrequency(SYSCLK_SOURCE source, const XTALSetup& xtalSetup, const PLLSetup& pllSetup);
    constexpr SYSCLKSetup(SYSCLK_SOURCE source, int32_t frequency)
    : m_source { source }
    , m_frequency { frequency }
    {}
};

constexpr int32_t SYSCLKSetup::determineFrequency(SYSCLK_SOURCE source, const XTALSetup& xtalSetup, const PLLSetup& pllSetup){
    switch(source){
        case SYSCLK_SOURCE::HBO:        return HBO::frequency;
        case SYSCLK_SOURCE::EXT_CLK:    return xtalSetup.getFrequency();
        case SYSCLK_SOURCE::PLL:        return pllSetup.getFrequency();
        case SYSCLK_SOURCE::XTAL:       return xtalSetup.getFrequency();
    }
}


template<SYSCLK_SOURCE source, const XTALSetup xtalSetup, const PLLSetup pllSetup>
constexpr SYSCLKSetup SYSCLKSetup::generateSetup(){
    constexpr int32_t frequency { determineFrequency(source, xtalSetup, pllSetup) };
    static_assert(frequency>0);
    static_assert(frequency<=100'000'000);
    return {source, frequency};
}

class ClockSetup{
public:
    template<const CLKGENConfigStruct config>
    static constexpr ClockSetup generateSetup();

    constexpr const PLLSetup& getPllSetup() const {
        return m_pllSetup;
    }
    constexpr const XTALSetup& getXtalSetup() const {
        return m_xtalSetup;
    }
    constexpr const SYSCLKSetup& getSysclkSetup() const {
        return m_sysclkSetup;
    }
    constexpr int32_t getSysclkFrequency() const {
        return m_sysclkSetup.getFrequency();
    }
    constexpr int32_t getAPB1Frequency() const {
        return m_sysclkSetup.getAPB1Frequency();
    }
    constexpr int32_t getAPB2Frequency() const {
        return m_sysclkSetup.getAPB2Frequency();
    }
private:
    const XTALSetup m_xtalSetup;
    const PLLSetup m_pllSetup;
    const SYSCLKSetup m_sysclkSetup;

    constexpr ClockSetup(XTALSetup xtalSetup, PLLSetup pllSetup, SYSCLKSetup sysclkSetup)
    : m_xtalSetup { xtalSetup }
    , m_pllSetup { pllSetup }
    , m_sysclkSetup { sysclkSetup }
    {}
};


template<const CLKGENConfigStruct config>
constexpr ClockSetup ClockSetup::generateSetup(){
    constexpr XTALSetup xtalSetup { XTALSetup::generateSetup<config.xtalMode, config.xtalFreq>() };
    constexpr PLLSetup pllSetup { PLLSetup::generateSetup<config.usePLL, xtalSetup, config.pllTargetFrequency, config.requireExactPLLFrequency>() };
    constexpr SYSCLKSetup sysclkSetup { SYSCLKSetup::generateSetup<config.sysclkSource, xtalSetup, pllSetup>() };
    return {
        xtalSetup,
        pllSetup,
        sysclkSetup 
    };
}
}
