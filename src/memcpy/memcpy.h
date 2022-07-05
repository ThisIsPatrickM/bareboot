#include <cstdint>

namespace bootloader::memcpy {

using namespace std;

/**
 * @brief Customized memcpy function. In Comparison to RODOS::memcpy it does not rely on copying
 * 1-byte-sized values.
 * Copies bytes using unsigned-sized values and the masks the remaining bytes.
 *
 * @note va41620 does not allow to assign 1-byte-sized values in code SRAM.
 * @todo Check if byteorder effects this implementation.
 * @todo Improve by aligning source and destination address.
 * @warning Don't use for overlapping memory regions
 *
 * @param dest pointer to destination
 * @param src pointer to source
 * @param n number of byte to copy
 * @return void* returns dest
 */
void* Memcpy(void* __restrict__ dest, const void* __restrict__ src, size_t n);

}
