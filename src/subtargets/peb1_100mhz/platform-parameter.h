/**
 * @file platform-parameter.h
 * @date 2020-01-28
 * @author Sebastian Wuerl
 *
 *
 * @brief configuration parameters ...
 *
 */

#pragma once

namespace RODOS {

/** Version Number */
#define OSVERSION "VA41620 Cortex-M4 V0.0"

/*************** System Configuration *********/

/** the following define sets the UART used for debug outputs with xprintf
 *
 */
#ifndef UART_DEBUG_IDX
#    define UART_DEBUG_IDX UART_IDX0
#    define UART_DEBUG_RX GPIO_096 // PG0
#    define UART_DEBUG_TX GPIO_097 // PG1
#endif

/** Memory for allocation (xmalloc) e.g. for all thread stacks ***/
#undef XMALLOC_SIZE
#define XMALLOC_SIZE 4 * 1024

/** default stack size (in bytes) for threads */
#undef DEFAULT_STACKSIZE
#define DEFAULT_STACKSIZE 2000

/** stack size (in bytes) for scheduler
 * - ISRs and Scheduler are using the main stack (MSP)
 * - The array "char __schedulerStack__[SCHEDULER_STACKSIZE];" in scheduler.cpp is not used!
 * - So we must provide enough stack in the linker script (see "_Min_Stack_Size" in stm32_flash.ld)!
 */
#undef SCHEDULER_STACKSIZE
#define SCHEDULER_STACKSIZE 8

/** time interval between timer interrupts in microseconds - max. 798000us (if CPU_CLK==168MHz)!!! */
#undef PARAM_TIMER_INTERVAL
#define PARAM_TIMER_INTERVAL 10000
}
