#pragma once

#include <cstdint>

namespace bootloader::checksums {

class Checksums {
public:
    static constexpr uint32_t CRC_INITIAL_VALUE = 0xFFFFFFFF;
    static constexpr uint32_t CRC_FINAL_XOR_VALUE = 0xFFFFFFFF;

    // TODO: Table Lookup, Document. Think about Memory. Maybe use Table in RODOS, but direct
    // function in bootloader. Maybe create a Tempalted class that does both and calculates Table?
    // https://lxp32.github.io/docs/a-simple-example-crc32-calculation/
    static uint32_t calculateCrc32NoTable(const unsigned char* message, uint32_t length)
    {
        uint32_t crc = CRC_INITIAL_VALUE;

        for (uint32_t i = 0; i < length; i++) {
            crc = crc ^ message[i]; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
            for (int j = 0; j < 8; j++) {
                if (crc & 1) { // NOLINT(readability-implicit-bool-conversion)
                    crc = (crc >> 1) ^ CRC_POLYNOMIAL;
                } else {
                    crc = crc >> 1;
                }
            }
        }

        return (crc ^ CRC_FINAL_XOR_VALUE);
    }

    static uint32_t calculateIterativeCrc32NoTable(
        const unsigned char* message, uint32_t length, uint32_t crc)
    {
        for (uint32_t i = 0; i < length; i++) {
            crc = crc ^ message[i]; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
            for (int j = 0; j < 8; j++) {
                if (crc & 1) { // NOLINT(readability-implicit-bool-conversion)
                    crc = (crc >> 1) ^ CRC_POLYNOMIAL;
                } else {
                    crc = crc >> 1;
                }
            }
        }
        return crc;
    }

private:
    static constexpr uint32_t CRC_POLYNOMIAL = 0xEDB88320;
};

}
