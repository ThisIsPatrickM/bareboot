#pragma once

#include <cstdint>

namespace RODOS {

constexpr uint32_t PERIPH_BASE = 0x4000'0000;
constexpr uint32_t PERIPH_BB_BASE = 0x4200'0000;

/**
 * @brief allows accessing a single bit of a register atomically with the bitbanding feature of the Cortex M4
 *
 * see Cortex-M4 Technical reference manual chapter 3.7
 *
 * note: because the bit-band region of the M4 only goes from 0x40000000 to 0x40100000, the
 * QUADSPI, FMC, RNG, HASH, AES, DCMI, ADC, OTG_FS and GPIO peripheral registers can't be used with this
 */
template <uint32_t baseAddr, uint32_t offset, typename T>
class HWPeriphBitBand {
    static constexpr uint32_t MIN_BITBAND_ADDR = 0x4000'0000;
    static constexpr uint32_t MAX_BITBAND_ADDR = 0x4010'0000;
    static_assert(baseAddr>=MIN_BITBAND_ADDR && baseAddr<MAX_BITBAND_ADDR);
public:
    static constexpr uint32_t ADDR = (PERIPH_BB_BASE + (baseAddr - PERIPH_BASE) * 32 + offset * 4);

    static inline T* const asObject = reinterpret_cast<T*>(ADDR);
};

}
