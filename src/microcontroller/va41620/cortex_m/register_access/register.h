#pragma once

#include "microcontroller/va41620/cortex_m/register_access/mmio.h"
#include "rodos_includes/bit_field.h"

namespace RODOS {

using Register = BitField<MMIO>;
using RegisterCopy = UInt32BitField;

template <uint32_t offset_, uint8_t length_ = 1> // NOLINT
using RegSubValue = UInt32SubValue<offset_, length_>;

using Register8 = BitField<MMIO8>;
using Register8Copy = UInt8BitField;

template <uint8_t offset_, uint8_t length_ = 1> // NOLINT
using RegSubValue8 = UInt8SubValue<offset_, length_>;

}
