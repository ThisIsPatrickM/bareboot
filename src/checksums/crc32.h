#pragma once

#include <cstdint>

namespace bootloader::checksums {

/**
 * @brief A CRC checksum class using a templated polynomial.
 * Computation can be improved by using a precalculated table, but increases memory consumption.
 *
 */
class Crc32 {
public:
    /**
     * @brief DEFAULT_POLYNOM matches the polynom used by a default Linux CLI "crc32 my_file"
     * and python library "zlib.crc32(...)"
     */
    static constexpr uint32_t DEFAULT_POLYNOM = 0xEDB88320;
    static constexpr uint32_t CRC_INITIAL_VALUE = 0xFFFFFFFF;
    static constexpr uint32_t CRC_FINAL_XOR_VALUE = 0xFFFFFFFF;

    /**
     * @brief Calculate CRC2 checksum for a given memory area
     *
     * @param message Memory address of start byte of area to do checksum.
     * @param length The length of the memory area to calculate checksum.
     * @param polynom The Polynom that will be used for calculation.
     * @return uint32_t Calculated CRC32 checksum in host byte order.
     */
    static uint32_t calculateCrc32(
        const uint8_t* message, uint32_t length, uint32_t polynom = DEFAULT_POLYNOM)
    {
        if (message == nullptr) {
            return 0;
        }
        uint32_t crc = CRC_INITIAL_VALUE; // NOLINT

        for (uint32_t i = 0; i < length; i++) {
            crc = crc ^ message[i]; // NOLINT
            for (int j = 0; j < 8; j++) {
                if (crc & 1) { // NOLINT
                    crc = (crc >> 1) ^ polynom; // NOLINT
                } else {
                    crc = crc >> 1;
                }
            }
        }

        return (crc ^ CRC_FINAL_XOR_VALUE);
    }

    /**
     * @brief Calculates CRC32 checksum for a given memory area with a given starting CRC.
     * Can be used to continue calculating CRC32 checksum, if the total memory area does not fit in
     * RAM.
     *
     * Usage:
     * - Caller needs to begin with CRC_INITIAL_VALUE
     * - Keep calling calculateCrc32Step() with memory areas that should be used to calculate CRC
     * and use previous result as crc input
     * - Once every memory area has been used, calculate (crc ^ CRC_FINAL_XOR_VALUE) with the last
     * output.
     *
     * @tparam POLYNOM The Polynom that will be used for calculation.
     * @param message Memory address of start byte of area to do checksum.
     * @param length The length of the memory area to calculate checksum.
     * @param crc Initial value to calculate CRC.
     * @tparam polynom The Polynom that will be used for calculation.
     * @return uint32_t New CRC after memory area.
     */
    static uint32_t calculateCrc32Step(
        const uint8_t* message, uint32_t length, uint32_t crc, uint32_t polynom = DEFAULT_POLYNOM)
    {
        if (message == nullptr) {
            return crc;
        }
        for (uint32_t i = 0; i < length; i++) {
            crc = crc ^ message[i]; // NOLINT
            for (int j = 0; j < 8; j++) {
                if (crc & 1) { // NOLINT
                    crc = (crc >> 1) ^ polynom; // NOLINT
                } else {
                    crc = crc >> 1;
                }
            }
        }
        return crc;
    }
};

}
