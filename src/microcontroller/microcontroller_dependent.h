#pragma once

#include "bootloader.h"
#include "memory_map.h"
#include "metadata.h"
#include <cstdint>

namespace bootloader {

static constexpr int32_t WATCHDOG_TIMEOUT_MS = 20000;

/**
 * @brief Move interrupt Vector table of application to expected location, or define Table Offset,
 * depending on architectural options.
 *
 */
void Move_Vector_Table();

/**
 * @brief Initialize Watchdog Timer to detect stuck in bootloader.
 *
 */
void Init_Watchdog();

void Trigger_Watchdog();

} // namespace bootloader
