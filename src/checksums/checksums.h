#pragma once

#include <cstdint>

namespace bootloader::checksums {

class Checksums {
public:
    // TODO: Table Lookup, Document. Think about Memory. Maybe use Table in RODOS, but direct
    // function in bootloader. Maybe create a Tempalted class that does both and calculates Table?
    // https://lxp32.github.io/docs/a-simple-example-crc32-calculation/
    static uint32_t calculateCrc32NoTable(const unsigned char* message, uint32_t length)
    {
        int i, j; // NOLINT
        unsigned byte, crc, mask; // NOLINT

        i = 0;
        crc = 0xFFFFFFFF; // NOLINT
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
};

}
