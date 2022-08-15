/**
 * @file    hal_spi.h
 *
 * @date    24.04.2013
 * @author  Michael Ruffer, Claudio S...
 *
 * @brief   Simple API to configure and use SPI interfaces
 */
#pragma once

#include <cstdint>
#include <new>

namespace RODOS {

constexpr uint32_t DEFAULT_BAUDRATE = 1000000;

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

    /**
     * @brief   Send and receive data FULL-DUPLEX.
     *          With every sent byte one byte is received and put in recBuf.
     *          If recBuf is bigger than sendBuf dummy bytes will be sent to
     *          get requested data.
     *          If sendBuf is bigger than recBuf you will only get the first
     *          received bytes until recBuf is full.
     *          Does not return until transfer is finished(blocking).
     * @param   sendBuf pointer to transmit buffer
     * @param   len     size of transmit buffer
     * @param   recBuf  pointer to receive buffer
     * @param   maxLen  size of receive buffer
     * @retval  int32_t number of received bytes, value < 0 on failure
     */
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
