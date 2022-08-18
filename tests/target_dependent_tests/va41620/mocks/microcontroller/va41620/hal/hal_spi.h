
#pragma once

#include <cstdint>
#include <new>

namespace RODOS {
enum SpiIdx { //  STM32F4
    SPI_IDX0, //  not available
    SPI_IDX1, //  SPI1
    SPI_IDX2, //  SPI2
    SPI_IDX3, //   SPI3
    SPI_IDX4, //  SPI4 (STM32F429 only)
    SPI_IDX5, //  SPI5 (STM32F429 only)
    SPI_IDX6 //  SPI6 (STM32F429 only)
};
class HalSpi {
public:
    HalSpi([[maybe_unused]] RODOS::SpiIdx spiIdx) {}

    void init([[maybe_unused]] uint32_t frequency) {}

    int32_t writeRead(
        [[maybe_unused]] const void* txBuffer,
        [[maybe_unused]] std::size_t txSize,
        [[maybe_unused]] void* rxBuffer,
        [[maybe_unused]] std::size_t rxSize)
    {
        return -1;
    }

    int32_t write([[maybe_unused]] const void* txBuffer, [[maybe_unused]] std::size_t txSize)
    {
        return -1;
    }
};
}
