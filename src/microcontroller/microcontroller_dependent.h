#pragma once

#include "bootloader.h"
#include "call_constructors.h"
#include "memory_map.h"
#include "metadata.h"
#include <cstdint>

namespace bootloader {

/**
 * @brief Move interrupt Vector table of application to expected location, or define Table Offset,
 * depending on architectural options.
 *
 */
void Move_Vector_Table();

/**
 * @brief Disable interrupts while Vector Table is moved.
 *
 */
void Disable_Interrupts();

/**
 * @brief Enable interrupts after Vector Table is moved.
 *
 */
void Enable_Interrupts();

/**
 * @brief Initialize Watchdog Timer to detect stuck in bootloader.
 *
 */
void Init_Watchdog();

// TODO Move to a class?

} // namespace bootloader
