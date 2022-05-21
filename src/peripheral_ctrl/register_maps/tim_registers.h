#pragma once
#include "cortex_m/register_access/register.h"
#include "cortex_m/register_access/reserved_bytes.h"

namespace RODOS {

namespace TIM_CTRL {
    using ENABLE = RegSubValue<0>;
    using ACTIVE = RegSubValue<1>;
    using AUTO_DISABLE = RegSubValue<2>;
    using AUTO_DEACTIVATE= RegSubValue<3>;
    using IRQ_ENB = RegSubValue<4>;
    using STATUS_SEL = RegSubValue<5, 4>;
    using STATUS_INV = RegSubValue<8>;
    using REQ_STOP= RegSubValue<9>;
}

namespace TIM_CSD_CTRL {
    using CSDEN0 = RegSubValue<0>;
    using CSDINV0 = RegSubValue<1>;
    using CSDEN1 = RegSubValue<2>;
    using CSDINV1 = RegSubValue<3>;
    using DCASOP = RegSubValue<4>;
    using CSDTRG0 = RegSubValue<6>;
    using CSDTRG1 = RegSubValue<7>;
    using CSDEN2 = RegSubValue<8>;
    using CSDINV2 = RegSubValue<9>;
    using CSDTRG2 = RegSubValue<10>;
}

class CascadeSource {
public:
    static constexpr CascadeSource fromTimer(int8_t timer){
        constexpr auto numTimers { 24 };
        if (timer<numTimers){
            return { static_cast<int8_t>(timer+80) };
        }
        __builtin_trap();
    }

    constexpr operator int8_t() const{
        return m_src;
    }
private:
    constexpr CascadeSource(int8_t s)
        : m_src{s}
    {}
    int8_t m_src;
};

struct TIMStruct {
    Register CTRL;
    Register RST_VALUE;
    Register CNT_VALUE;
    Register ENABLE;
    Register CSD_CTRL;
    Register CASCADE[3];
    Register PWMA_VALUE;
    Register PWMB_VALUE;
    ReservedWord RESERVED0[(0x3FC-0x028)/sizeof(ReservedWord)];
    Register PERID;
};

static_assert(sizeof(TIMStruct)==0x400);

}
