#pragma once

#include <cstdint>

namespace bootloader::checksums {

class Checksums {
public:
    static constexpr uint32_t CRC_INITIAL_VALUE = 0xFFFFFFFF;

    // TODO: Table Lookup, Document. Think about Memory. Maybe use Table in RODOS, but direct
    // function in bootloader. Maybe create a Tempalted class that does both and calculates Table?
    // https://lxp32.github.io/docs/a-simple-example-crc32-calculation/
    static uint32_t calculateCrc32NoTable(const unsigned char* message, uint32_t length)
    {
        int i, j; // NOLINT
        unsigned byte, crc, mask; // NOLINT

        i = 0;
        crc = CRC_INITIAL_VALUE; // NOLINT
        while (length-- != 0) {
            byte = message[i]; // Get next byte.// NOLINT
            crc = crc ^ byte;
            for (j = 7; j >= 0; j--) { // Do eight times.
                mask = -(crc & 1);
                crc = (crc >> 1) ^ (0xEDB88320 & mask); // NOLINT
            }
            i = i + 1;
        }
        return ~crc;
    }

    static uint32_t calculateIterativeCrc32NoTable(
        const unsigned char* message, uint32_t length, uint32_t crc)
    {
        int i, j; // NOLINT
        unsigned int byte, mask; // NOLINT

        i = 0;
        while (length-- != 0) {
            byte = message[i]; // Get next byte.// NOLINT
            crc = crc ^ byte;
            for (j = 7; j >= 0; j--) { // Do eight times.
                mask = -(crc & 1);
                crc = (crc >> 1) ^ (0xEDB88320 & mask); // NOLINT
            }
            i = i + 1;
        }
        return crc;
    }
};

}
