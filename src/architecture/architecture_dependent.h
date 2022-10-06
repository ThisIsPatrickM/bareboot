#pragma once

#include "memory_map.h"

namespace bootloader::architecture {

/**
 * @brief Hand over control to the application
 *
 */
void Start_App();

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

} // namespace bootloader::architecture
