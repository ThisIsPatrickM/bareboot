#include "microcontroller/microcontroller_dependent.h"

/* Controller specific includes */
#include "architecture/architecture_dependent.h"
#include "microcontroller/va41620/irq_handlers.h"
#include "platform_parameters.h"

namespace bootloader {

using namespace bootloader::va41620;
using namespace bootloader::architecture;

/*******************************************************************************
 * Global Functions for that every Microcontroller needs to implement
 ******************************************************************************/
void Move_Vector_Table()
{
    constexpr uint32_t TBLOFF = 0xE000ED08;
    /* Set the vector table base address */
    uint32_t* vector_table = (uint32_t*)&__approm_start__; // NOLINT
    auto* vtor = reinterpret_cast<uint32_t*>(TBLOFF); // NOLINT
    *vtor = ((uintptr_t)vector_table & 0xFFFFFFF8); // NOLINT
    // TODO If anything fails, try this:
    // *vtor = ((uint32_t)vector_table & 0xFFFFFFF8); // NOLINT

    // Hardcoded to boot Image 2:
    // TODO Dont hardcode
    // uint32_t* vector_table = (uint32_t*)68608; // NOLINT
    // auto* vtor = reinterpret_cast<uint32_t*>(TBLOFF); // NOLINT
    // *vtor = ((uintptr_t)vector_table & 0xFFFFFFF8); // NOLINT
}

void Toggle_ROM_Writeable(bool writeable)
{
    constexpr uint32_t ROM_PROT_ADDRESS = 0x40010010;
    uint32_t* ROM_PROT = reinterpret_cast<uint32_t*>(ROM_PROT_ADDRESS); // NOLINT
    if (writeable) {
        *ROM_PROT = 0x1;
    } else {
        *ROM_PROT = 0x0;
    }
}

void Disable_Interrupts()
{
    // TODO
}

void Enable_Interrupts()
{
    // TODO
}

void Init_Watchdog()
{
    // TODO
}

/*******************************************************************************
 * Global Functions for Reset Handler
 ******************************************************************************/

void Init_Data_Section()
{
    volatile uint32_t* destPtr = _sdata;
    const volatile uint32_t* srcPtr = _sidata;
    while (reinterpret_cast<uintptr_t>(destPtr) < reinterpret_cast<uintptr_t>(_edata)) { // NOLINT
        *destPtr = *srcPtr;
        destPtr++; // NOLINT
        srcPtr++; // NOLINT
    }
}

void Init_Bss_Section()
{
    volatile uint32_t* destPtr = _sbss;
    while (reinterpret_cast<uintptr_t>(destPtr) < reinterpret_cast<uintptr_t>(_ebss)) { // NOLINT
        *destPtr = 0;
        destPtr++; // NOLINT
    }
}

void Wait_About_5_Seconds()
{
    constexpr int32_t seconds { 5 }; // NOLINT
    constexpr int32_t frequencyHz { 20'000'000 }; // NOLINT
    constexpr int32_t approximateCyclesPerLoop { 3 }; // NOLINT
    constexpr int32_t totalLoops { (frequencyHz / approximateCyclesPerLoop) * seconds }; // NOLINT

    for (int32_t i { 0 }; i < totalLoops; i++) {
        asm volatile("");
    }
}

/*******************************************************************************
 * Entry Point from Linker
 ******************************************************************************/

extern "C" [[noreturn, gnu::used]] void Reset_Handler()
{
    // TODO remove before flight
    Wait_About_5_Seconds();

    Init_Watchdog();

    Memory_Barrier();

    Init_Data_Section();
    Init_Bss_Section();

    Memory_Barrier();
    Call_Constructors();
    Memory_Barrier();

    Bootloader boot {};
    boot.run();

    while (true) {
        ;
    }
}

/*******************************************************************************
 * Structs placed with Linker
 ******************************************************************************/

// constinit const GlobalImageMetadata GLOBAL_IMAGE_METADATA
//     __attribute__((used, section(".metadata_table"))) { .images {
//         { .imageBegin = bootloader::PlatformParameters::IMAGE_BEGIN_ADDRESSES[0] },
//         { .imageBegin = bootloader::PlatformParameters::IMAGE_BEGIN_ADDRESSES[1] },
//         { .imageBegin = bootloader::PlatformParameters::IMAGE_BEGIN_ADDRESSES[2] } } };

// constinit const struct IrqTable { // NOLINT
//     const volatile void* sp = _estack;

//     void (*resetHandler)() = Reset_Handler; // Reset Handler
//     void (*nmiHandler)() = NMI_Handler; // NMI Handler
//     void (*hardfaultHandler)() = HardFault_Handler; // Hard Fault Handler
//     void (*memmanageHandler)() = MemManage_Handler; // MPU Fault Handler
//     void (*busfaultHandler)() = BusFault_Handler; // Bus Fault Handler
//     void (*usagefaultHandler)() = UsageFault_Handler; // Usage Fault Handler
//     uintptr_t reserved0 = 0; // Reserved
//     uintptr_t reserved1 = 0; // Reserved
//     uintptr_t reserved2 = 0; // Reserved
//     uintptr_t reserved3 = 0; // Reserved
//     void (*svcHandler)() = SVC_Handler; // SVCall Handler
//     void (*debugmonHandler)() = DebugMon_Handler; // Debug Monitor Handler
//     uintptr_t reserved = 0; // Reserved
//     void (*pendsvHandler)() = PendSV_Handler; // PendSV Handler
//     void (*systickHandler)() = SysTick_Handler; // SysTick Handler

//     uintptr_t alwaysZero0 = 0; // 0: Always 0
//     uintptr_t alwaysZero1 = 0; // 1: Always 0
//     uintptr_t alwaysZero2 = 0; // 2: Always 0
//     uintptr_t alwaysZero3 = 0; // 3: Always 0
//     uintptr_t alwaysZero4 = 0; // 4: Always 0
//     uintptr_t alwaysZero5 = 0; // 5: Always 0
//     uintptr_t alwaysZero6 = 0; // 6: Always 0
//     uintptr_t alwaysZero7 = 0; // 7: Always 0
//     uintptr_t alwaysZero8 = 0; // 8: Always 0
//     uintptr_t alwaysZero9 = 0; // 9: Always 0
//     uintptr_t alwaysZero10 = 0; // 10: Always 0
//     uintptr_t alwaysZero11 = 0; // 11: Always 0
//     uintptr_t alwaysZero12 = 0; // 12: Always 0
//     uintptr_t alwaysZero13 = 0; // 13: Always 0
//     uintptr_t alwaysZero14 = 0; // 14: Always 0
//     uintptr_t alwaysZero15 = 0; // 15: Always 0
//     void (*spi0TxIrqHandler)() = SPI0_TX_IRQHandler; // 16: SPI0 TX
//     void (*spi0RxIrqHandler)() = SPI0_RX_IRQHandler; // 17: SPI0 RX
//     void (*spi1TxIrqHandler)() = SPI1_TX_IRQHandler; // 18: SPI1 TX
//     void (*spi1RxIrqHandler)() = SPI1_RX_IRQHandler; // 19: SPI1 RX
//     void (*spi2TxIrqHandler)() = SPI2_TX_IRQHandler; // 20: SPI2 TX
//     void (*spi2RxIrqHandler)() = SPI2_RX_IRQHandler; // 21: SPI2 RX
//     void (*spi3TxIrqHandler)() = SPI3_TX_IRQHandler; // 22: SPI3 TX
//     void (*spi3RxIrqHandler)() = SPI3_RX_IRQHandler; // 23: SPI3 RX
//     void (*uart0TxIrqHandler)() = UART0_TX_IRQHandler; // 24: UART0 TX
//     void (*uart0RxIrqHandler)() = UART0_RX_IRQHandler; // 25: UART0 RX
//     void (*uart1TxIrqHandler)() = UART1_TX_IRQHandler; // 26: UART1 TX
//     void (*uart1RxIrqHandler)() = UART1_RX_IRQHandler; // 27: UART1 RX
//     void (*uart2TxIrqHandler)() = UART2_TX_IRQHandler; // 28: UART2 TX
//     void (*uart2RxIrqHandler)() = UART2_RX_IRQHandler; // 29: UART2 RX
//     void (*i2c0MsIrqHandler)() = I2C0_MS_IRQHandler; // 30: I2C0_MS
//     void (*i2c0SlIrqHandler)() = I2C0_SL_IRQHandler; // 31: I2C0_SL
//     void (*i2c1MsIrqHandler)() = I2C1_MS_IRQHandler; // 32: I2C1_MS
//     void (*i2c1SlIrqHandler)() = I2C1_SL_IRQHandler; // 33: I2C1_SL
//     void (*i2c2MsIrqHandler)() = I2C2_MS_IRQHandler; // 34: I2C2_MS
//     void (*i2c2SlIrqHandler)() = I2C2_SL_IRQHandler; // 35: I2C2_SL
//     void (*etherTxIrqHandler)() = Ether_TX_IRQHandler; // 36: Ethernet TX
//     uintptr_t alwaysZero37 = 0; // 37: Always 0
//     void (*spwIrqHandler)() = SpW_IRQHandler; // 38: Space Wire
//     uintptr_t alwaysZero39 = 0; // 39: Always 0
//     void (*dac0IrqHandler)() = DAC0_IRQHandler; // 40: DAC 0
//     void (*dac1IrqHandler)() = DAC1_IRQHandler; // 41: DAC 1
//     void (*trngIrqHandler)() = TRNG_IRQHandler; // 42: Random Number Generator
//     void (*dmaErrorIrqHandler)() = DMA_Error_IRQHandler; // 43: DMA error
//     void (*adcIrqHandler)() = ADC_IRQHandler; // 44: ADC
//     void (*loclkIrqHandler)() = LoCLK_IRQHandler; // 45: LoCLK
//     void (*lvdIrqHandler)() = LVD_IRQHandler; // 46: LVD
//     void (*wdtIrqHandler)() = WDT_IRQHandler; // 47: Watchdog
//     void (*tim0IrqHandler)() = TIM0_IRQHandler; // 48: Timer 0
//     void (*tim1IrqHandler)() = TIM1_IRQHandler; // 49: Timer 1
//     void (*tim2IrqHandler)() = TIM2_IRQHandler; // 50: Timer 2
//     void (*tim3IrqHandler)() = TIM3_IRQHandler; // 51: Timer 3
//     void (*tim4IrqHandler)() = TIM4_IRQHandler; // 52: Timer 4
//     void (*tim5IrqHandler)() = TIM5_IRQHandler; // 53: Timer 5
//     void (*tim6IrqHandler)() = TIM6_IRQHandler; // 54: Timer 6
//     void (*tim7IrqHandler)() = TIM7_IRQHandler; // 55: Timer 7
//     void (*tim8IrqHandler)() = TIM8_IRQHandler; // 56: Timer 8
//     void (*tim9IrqHandler)() = TIM9_IRQHandler; // 57: Timer 9
//     void (*tim10IrqHandler)() = TIM10_IRQHandler; // 58: Timer 10
//     void (*tim11IrqHandler)() = TIM11_IRQHandler; // 59: Timer 11
//     void (*tim12IrqHandler)() = TIM12_IRQHandler; // 60: Timer 12
//     void (*tim13IrqHandler)() = TIM13_IRQHandler; // 61: Timer 13
//     void (*tim14IrqHandler)() = TIM14_IRQHandler; // 62: Timer 14
//     void (*tim15IrqHandler)() = TIM15_IRQHandler; // 63: Timer 15
//     void (*tim16IrqHandler)() = TIM16_IRQHandler; // 64: Timer 16
//     void (*tim17IrqHandler)() = TIM17_IRQHandler; // 65: Timer 17
//     void (*tim18IrqHandler)() = TIM18_IRQHandler; // 66: Timer 18
//     void (*tim19IrqHandler)() = TIM19_IRQHandler; // 67: Timer 19
//     void (*tim20IrqHandler)() = TIM20_IRQHandler; // 68: Timer 20
//     void (*tim21IrqHandler)() = TIM21_IRQHandler; // 69: Timer 21
//     void (*tim22IrqHandler)() = TIM22_IRQHandler; // 70: Timer 22
//     void (*tim23IrqHandler)() = TIM23_IRQHandler; // 71: Timer 23
//     void (*can0IrqHandler)() = CAN0_IRQHandler; // 72: CAN 0
//     uintptr_t alwaysZero73 = 0; // 73: Always 0
//     void (*can1IrqHandler)() = CAN1_IRQHandler; // 74: CAN 1
//     uintptr_t alwaysZero75 = 0; // 75: Always 0
//     void (*edacMbeIrqHandler)() = EDAC_MBE_IRQHandler; // 76: EDAC Multi Bit Error
//     void (*edacSbeIrqHandler)() = EDAC_SBE_IRQHandler; // 77: EDAC Single Bit Error
//     void (*pa0IrqHandler)() = PA0_IRQHandler; // 78: PORTA 0
//     void (*pa1IrqHandler)() = PA1_IRQHandler; // 79: PORTA 1
//     void (*pa2IrqHandler)() = PA2_IRQHandler; // 80: PORTA 2
//     void (*pa3IrqHandler)() = PA3_IRQHandler; // 81: PORTA 3
//     void (*pa4IrqHandler)() = PA4_IRQHandler; // 82: PORTA 4
//     void (*pa5IrqHandler)() = PA5_IRQHandler; // 83: PORTA 5
//     void (*pa6IrqHandler)() = PA6_IRQHandler; // 84: PORTA 6
//     void (*pa7IrqHandler)() = PA7_IRQHandler; // 85: PORTA 7
//     void (*pa8IrqHandler)() = PA8_IRQHandler; // 86: PORTA 8
//     void (*pa9IrqHandler)() = PA9_IRQHandler; // 87: PORTA 9
//     void (*pa10IrqHandler)() = PA10_IRQHandler; // 88: PORTA 10
//     void (*pa11IrqHandler)() = PA11_IRQHandler; // 89: PORTA 11
//     void (*pa12IrqHandler)() = PA12_IRQHandler; // 90: PORTA 12
//     void (*pa13IrqHandler)() = PA13_IRQHandler; // 91: PORTA 13
//     void (*pa14IrqHandler)() = PA14_IRQHandler; // 92: PORTA 14
//     void (*pa15IrqHandler)() = PA15_IRQHandler; // 93: PORTA 15
//     void (*pb0IrqHandler)() = PB0_IRQHandler; // 94: PORTB 0
//     void (*pb1IrqHandler)() = PB1_IRQHandler; // 95: PORTB 1
//     void (*pb2IrqHandler)() = PB2_IRQHandler; // 96: PORTB 2
//     void (*pb3IrqHandler)() = PB3_IRQHandler; // 97: PORTB 3
//     void (*pb4IrqHandler)() = PB4_IRQHandler; // 98: PORTB 4
//     void (*pb5IrqHandler)() = PB5_IRQHandler; // 99: PORTB 5
//     void (*pb6IrqHandler)() = PB6_IRQHandler; // 100: PORTB 6
//     void (*pb7IrqHandler)() = PB7_IRQHandler; // 101: PORTB 7
//     void (*pb8IrqHandler)() = PB8_IRQHandler; // 102: PORTB 8
//     void (*pb9IrqHandler)() = PB9_IRQHandler; // 103: PORTB 9
//     void (*pb10IrqHandler)() = PB10_IRQHandler; // 104: PORTB 10
//     void (*pb11IrqHandler)() = PB11_IRQHandler; // 105: PORTB 11
//     void (*pb12IrqHandler)() = PB12_IRQHandler; // 106: PORTB 12
//     void (*pb13IrqHandler)() = PB13_IRQHandler; // 107: PORTB 13
//     void (*pb14IrqHandler)() = PB14_IRQHandler; // 108: PORTB 14
//     void (*pb15IrqHandler)() = PB15_IRQHandler; // 109: PORTB 15
//     void (*pc0IrqHandler)() = PC0_IRQHandler; // 110: PORTC 0
//     void (*pc1IrqHandler)() = PC1_IRQHandler; // 111: PORTC 1
//     void (*pc2IrqHandler)() = PC2_IRQHandler; // 112: PORTC 2
//     void (*pc3IrqHandler)() = PC3_IRQHandler; // 113: PORTC 3
//     void (*pc4IrqHandler)() = PC4_IRQHandler; // 114: PORTC 4
//     void (*pc5IrqHandler)() = PC5_IRQHandler; // 115: PORTC 5
//     void (*pc6IrqHandler)() = PC6_IRQHandler; // 116: PORTC 6
//     void (*pc7IrqHandler)() = PC7_IRQHandler; // 117: PORTC 7
//     void (*pc8IrqHandler)() = PC8_IRQHandler; // 118: PORTC 8
//     void (*pc9IrqHandler)() = PC9_IRQHandler; // 119: PORTC 9
//     void (*pc10IrqHandler)() = PC10_IRQHandler; // 120: PORTC 10
//     void (*pc11IrqHandler)() = PC11_IRQHandler; // 121: PORTC 11
//     void (*pc12IrqHandler)() = PC12_IRQHandler; // 122: PORTC 12
//     void (*pc13IrqHandler)() = PC13_IRQHandler; // 123: PORTC 13
//     void (*pc14IrqHandler)() = PC14_IRQHandler; // 124: PORTC 14
//     void (*pc15IrqHandler)() = PC15_IRQHandler; // 125: PORTC 15
//     void (*pd0IrqHandler)() = PD0_IRQHandler; // 126: PORTD 0
//     void (*pd1IrqHandler)() = PD1_IRQHandler; // 127: PORTD 1
//     void (*pd2IrqHandler)() = PD2_IRQHandler; // 128: PORTD 2
//     void (*pd3IrqHandler)() = PD3_IRQHandler; // 129: PORTD 3
//     void (*pd4IrqHandler)() = PD4_IRQHandler; // 130: PORTD 4
//     void (*pd5IrqHandler)() = PD5_IRQHandler; // 131: PORTD 5
//     void (*pd6IrqHandler)() = PD6_IRQHandler; // 132: PORTD 6
//     void (*pd7IrqHandler)() = PD7_IRQHandler; // 133: PORTD 7
//     void (*pd8IrqHandler)() = PD8_IRQHandler; // 134: PORTD 8
//     void (*pd9IrqHandler)() = PD9_IRQHandler; // 135: PORTD 9
//     void (*pd10IrqHandler)() = PD10_IRQHandler; // 136: PORTD 10
//     void (*pd11IrqHandler)() = PD11_IRQHandler; // 137: PORTD 11
//     void (*pd12IrqHandler)() = PD12_IRQHandler; // 138: PORTD 12
//     void (*pd13IrqHandler)() = PD13_IRQHandler; // 139: PORTD 13
//     void (*pd14IrqHandler)() = PD14_IRQHandler; // 140: PORTD 14
//     void (*pd15IrqHandler)() = PD15_IRQHandler; // 141: PORTD 15
//     void (*pe0IrqHandler)() = PE0_IRQHandler; // 142: PORTE 0
//     void (*pe1IrqHandler)() = PE1_IRQHandler; // 143: PORTE 1
//     void (*pe2IrqHandler)() = PE2_IRQHandler; // 144: PORTE 2
//     void (*pe3IrqHandler)() = PE3_IRQHandler; // 145: PORTE 3
//     void (*pe4IrqHandler)() = PE4_IRQHandler; // 146: PORTE 4
//     void (*pe5IrqHandler)() = PE5_IRQHandler; // 147: PORTE 5
//     void (*pe6IrqHandler)() = PE6_IRQHandler; // 148: PORTE 6
//     void (*pe7IrqHandler)() = PE7_IRQHandler; // 149: PORTE 7
//     void (*pe8IrqHandler)() = PE8_IRQHandler; // 150: PORTE 8
//     void (*pe9IrqHandler)() = PE9_IRQHandler; // 151: PORTE 9
//     void (*pe10IrqHandler)() = PE10_IRQHandler; // 152: PORTE 10
//     void (*pe11IrqHandler)() = PE11_IRQHandler; // 153: PORTE 11
//     void (*pe12IrqHandler)() = PE12_IRQHandler; // 154: PORTE 12
//     void (*pe13IrqHandler)() = PE13_IRQHandler; // 155: PORTE 13
//     void (*pe14IrqHandler)() = PE14_IRQHandler; // 156: PORTE 14
//     void (*pe15IrqHandler)() = PE15_IRQHandler; // 157: PORTE 15
//     void (*pf0IrqHandler)() = PF0_IRQHandler; // 158: PORTF 0
//     void (*pf1IrqHandler)() = PF1_IRQHandler; // 159: PORTF 1
//     void (*pf2IrqHandler)() = PF2_IRQHandler; // 160: PORTF 2
//     void (*pf3IrqHandler)() = PF3_IRQHandler; // 161: PORTF 3
//     void (*pf4IrqHandler)() = PF4_IRQHandler; // 162: PORTF 4
//     void (*pf5IrqHandler)() = PF5_IRQHandler; // 163: PORTF 5
//     void (*pf6IrqHandler)() = PF6_IRQHandler; // 164: PORTF 6
//     void (*pf7IrqHandler)() = PF7_IRQHandler; // 165: PORTF 7
//     void (*pf8IrqHandler)() = PF8_IRQHandler; // 166: PORTF 8
//     void (*pf9IrqHandler)() = PF9_IRQHandler; // 167: PORTF 9
//     void (*pf10IrqHandler)() = PF10_IRQHandler; // 168: PORTF 10
//     void (*pf11IrqHandler)() = PF11_IRQHandler; // 169: PORTF 11
//     void (*pf12IrqHandler)() = PF12_IRQHandler; // 170: PORTF 12
//     void (*pf13IrqHandler)() = PF13_IRQHandler; // 171: PORTF 13
//     void (*pf14IrqHandler)() = PF14_IRQHandler; // 172: PORTF 14
//     void (*pf15IrqHandler)() = PF15_IRQHandler; // 173: PORTF 15
//     void (*dmaActive0IrqHandler)() = DMA_Active_0_IRQHandler; // 174: DMA Active 0
//     void (*dmaActive1IrqHandler)() = DMA_Active_1_IRQHandler; // 175: DMA Active 1
//     void (*dmaActive2IrqHandler)() = DMA_Active_2_IRQHandler; // 176: DMA Active 2
//     void (*dmaActive3IrqHandler)() = DMA_Active_3_IRQHandler; // 177: DMA Active 3
//     void (*dmaDone0IrqHandler)() = DMA_Done_0_IRQHandler; // 178: DMA Done 0
//     void (*dmaDone1IrqHandler)() = DMA_Done_1_IRQHandler; // 179: DMA Done 1
//     void (*dmaDone2IrqHandler)() = DMA_Done_2_IRQHandler; // 180: DMA Done 2
//     void (*dmaDone3IrqHandler)() = DMA_Done_3_IRQHandler; // 181: DMA Done 3
//     void (*i2c0MsRxIrqHandler)() = I2C0_MS_RX_IRQHandler; // 182: I2C0 Master RX
//     void (*i2c0MsTxIrqHandler)() = I2C0_MS_TX_IRQHandler; // 183: I2C0 Master TX
//     void (*i2c0SlRxIrqHandler)() = I2C0_SL_RX_IRQHandler; // 184: I2C0 Slave RX
//     void (*i2c0SlTxIrqHandler)() = I2C0_SL_TX_IRQHandler; // 185: I2C0 Slave TX
//     void (*i2c1MsRxIrqHandler)() = I2C1_MS_RX_IRQHandler; // 186: I2C1 Master RX
//     void (*i2c1MsTxIrqHandler)() = I2C1_MS_TX_IRQHandler; // 187: I2C1 Master TX
//     void (*i2c1SlRxIrqHandler)() = I2C1_SL_RX_IRQHandler; // 188: I2C1 Slave RX
//     void (*i2c1SlTxIrqHandler)() = I2C1_SL_TX_IRQHandler; // 189: I2C1 Slave TX
//     void (*i2c2MsRxIrqHandler)() = I2C2_MS_RX_IRQHandler; // 190: I2C2 Master RX
//     void (*i2c2MsTxIrqHandler)() = I2C2_MS_TX_IRQHandler; // 191: I2C2 Master TX
//     void (*i2c2SlRxIrqHandler)() = I2C2_SL_RX_IRQHandler; // 192: I2C2 Slave RX
//     void (*i2c2SlTxIrqHandler)() = I2C2_SL_TX_IRQHandler; // 193: I2C2 Slave TX
//     void (*fpuIrqHandler)() = FPU_IRQHandler; // 194: FPU
//     void (*txevIrqHandler)() = TXEV_IRQHandler; // 195: TXEV
// } irqTable __attribute__((used, section(".isr_vector"))); // NOLINT

} // namespace bootloader
