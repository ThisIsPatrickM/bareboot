// #pragma once
// #include <cstdint>
// #include <cstddef>

// namespace rodos {

// class NVIC{
// private:
//     static constexpr size_t priorityBits = 4; /// priority bits supported by STM32 CM4 uCs. See
//     STM32 Cortex M4 Programming manual Table 47
// public:
//     static constexpr uint8_t lowestPriority = ((1<<priorityBits)-1);
//     static constexpr uint8_t defaultPriority = lowestPriority/2;
//     static constexpr uint8_t highestPriority = 0;

//     static constexpr uint8_t numInterrupts = 91;

//     enum class SYSTEM_HANDLER {
//         RESET = 1,
//         NMI = 2,
//         HARD_FAULT = 3,
//         MEM_MANAGE = 4,
//         BUS_FAULT = 5,
//         USAGE_FAULT = 6,
//         SVCALL = 11,
//         DEBUG_MONITOR = 12,
//         PENDSV = 14,
//         SYSTICK = 15
//     };

//     static void enableIRQ(uint8_t irq);
//     static void disableIRQ(uint8_t irq);
//     static void clearPendingIRQ(uint8_t irq);
//     static void setIRQPriority(uint8_t irq, uint8_t priority);
//     static void setSystemHandlerPriority(SYSTEM_HANDLER irq, uint8_t priority);

//     static void setAllIrqsToDefaultPriority();

//     static void systemReset();
// };

// }
