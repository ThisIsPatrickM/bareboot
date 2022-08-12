/**
 * @file    hal_spi.h
 *
 * @date    24.04.2013
 * @author  Michael Ruffer, Claudio S...
 *
 * @brief   Simple API to configure and use SPI interfaces
 */
#pragma once

#include "microcontroller/va41620/hw_hal/hw_hal_spi.h"
#include <cstdint>
#include <new>

namespace RODOS {

constexpr uint32_t DEFAULT_BAUDRATE = 1000000;

class HalSpi {
    HwHalSpi m_context;

public:
    HalSpi(RODOS::SpiIdx spiIdx);

    /**
     * @brief   Initialization of SPI interface & pins
     *          Baudrate is set to the closest possible.
     *          Request configured baudrate with status().
     */
    void init(uint32_t baudrate = DEFAULT_BAUDRATE);

    /**
     * @brief   Send data to a spi slave device.
     *          Does not return until transfer is finished(blocking).
     * @param   sendBuf pointer to transmit buffer
     * @param   len     size of transmit buffer
     * @retval  int32_t number of sent bytes, value < 0 on failure
     */
    int32_t write(const void* txBuffer, size_t txSize);

    /**
     * @brief   Request data from a spi slave device.
     *          To get data from slave the master must generate the SPI clock and dummy
     *          data must be sent. For it recBuf will be used.
     *          Does not return until transfer is finished(blocking).
     * @param   recBuf  pointer to receive buffer
     * @param   maxLen  size of receive buffer
     * @retval  int32_t number of received bytes, value < 0 on failure
     */
    int32_t read(void* rxBuffer, size_t rxSize);

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
    int32_t writeRead(const void* txBuffer, size_t txSize, void* rxBuffer, size_t rxSize);
};

}
