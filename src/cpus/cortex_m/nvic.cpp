#include "nvic.h"
#include "misc-rodos-funcs.h"
#include "peripheral_defs.h"

namespace RODOS {

void NVIC::enableIRQ(uint8_t irq){
    nvic.ISER[irq/32].write(1ul<<(irq%32));
}

void NVIC::disableIRQ(uint8_t irq){
    nvic.ICER[irq/32].write(1ul<<(irq%32));
}

void NVIC::clearPendingIRQ(uint8_t irq){
    nvic.ICPR[irq/32].write(1ul<<(irq%32));
}

void NVIC::setIRQPriority(uint8_t irq, uint8_t priority){
    using namespace NVIC_IPR;
    nvic.IPR[irq].write(PRIO(priority));
}

void NVIC::setSystemHandlerPriority(SYSTEM_HANDLER irq, uint8_t priority){
    using namespace NVIC_IPR;
    scb.SHPR[static_cast<uint8_t>(irq)-4].write(PRIO(priority));
}

void NVIC::systemReset(){
    using namespace SCB_AIRCR;
    hwDisableInterrupts();
    asm volatile("dsb");
    scb.AIRCR.write(SYSRESETREQ(1), VECTKEY(VECTKEY_WRITEVALUE));
    asm volatile("dsb");
    while(1);
}

void NVIC::setAllIrqsToDefaultPriority(){
    for (uint8_t i = 0; i<numInterrupts; i++){
        setIRQPriority(i, defaultPriority);
    }
}


}
