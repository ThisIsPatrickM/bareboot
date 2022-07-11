#include <bit>
#include <cstdint>

namespace bootloader::memcpy {

/**
 * @brief Customized memcpy function. In Comparison to RODOS::memcpy it does not rely on copying
 * 1-byte-sized values.
 * Copies bytes using unsigned-sized values and masks the remaining bytes.
 *
 * @note va41620 does not allow to assign 1-byte-sized values in code SRAM.
 * @todo Improve by aligning source/destination address.
 * @warning Don't use for overlapping memory regions
 *
 * @param dest pointer to destination
 * @param src pointer to source
 * @param n number of byte to copy
 * @return void* returns dest
 */
void* va41620UnsignedMemcpy( // NOLINT
    void* __restrict__ dest,
    const void* __restrict__ src,
    std::size_t n);

}

namespace RODOS {

void* memcpy(void* dest, const void* src, std::size_t len); // NOLINT

} // namespace
