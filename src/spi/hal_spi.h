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
// #include "genericIO.h"
// #include "hal_gpio.h"
#include "hw_hal_spi.h"
#include "rodos_includes/hal_gpio.h"

namespace RODOS {

using namespace std;

// class HAL_SPI : public GenericIOInterface {
class HAL_SPI {
    HW_HAL_SPI* context;

public:
    HAL_SPI(SPI_IDX spiIdx);
    HAL_SPI(
        SPI_IDX idx,
        GPIO_PIN sckPin,
        GPIO_PIN misoPin,
        GPIO_PIN mosiPin,
        GPIO_PIN nssPin = GPIO_INVALID);
    virtual ~HAL_SPI() {}

    /**
     * @brief   Initialization of SPI interface & pins
     *          Baudrate is set to the closest possible.
     *          Request configured baudrate with status().
     * @return  0 on success, -1 on failure
     */
    int32_t init(uint32_t baudrate = 1000000, bool slave = false, bool tiMode = false);

    /**
     *  @brief  Disable interface and set all its registers and pins to its reset state.
     *          Before using the interface again you must call init().
     */
    void reset();

    /**
     * @brief   Configuration of SPI interface AFTER initialization
     * @return  0 on success, -1 on failure
     */
    int32_t config(SPI_PARAMETER_TYPE type, int32_t value);

    /**
     * @brief   request status of interface e.g. baudrate
     * @return  requested status value, -1 on failure
     */
    int32_t status(SPI_STATUS_TYPE type);

    /**
     * @brief   request interface sent status
     * @return  true -> new transmit can be started, false -> busy
     */
    bool isWriteFinished();

    /**
     * @brief   request interface receive status
     * @return  true -> previously started receive request is finished, false -> busy
     */
    bool isReadFinished();

    /**
     * @brief   Send data to a spi slave device.
     *          Does not return until transfer is finished(blocking).
     * @param   sendBuf pointer to transmit buffer
     * @param   len     size of transmit buffer
     * @retval  int32_t number of sent bytes, value < 0 on failure
     */
    int32_t write(const void* sendBuf, size_t len);

    /**
     * @brief   Request data from a spi slave device.
     *          To get data from slave the master must generate the SPI clock and dummy
     *          data must be sent. For it recBuf will be used.
     *          Does not return until transfer is finished(blocking).
     * @param   recBuf  pointer to receive buffer
     * @param   maxLen  size of receive buffer
     * @retval  int32_t number of received bytes, value < 0 on failure
     */
    int32_t read(void* recBuf, size_t maxLen);

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
    int32_t writeRead(const void* sendBuf, size_t len, void* recBuf, size_t maxLen);

    /**
     * @brief   Send data to a spi slave device.
     *          Define byte, which, once received on the miso line, triggers
     *          the start of the transmission.
     *          Alternatively possible to have the function trigger once this
     *          byte is not received anymore.
     *          Does not return until transfer is finished(blocking).
     * @param   sendBuf pointer to transmit buffer
     * @param   len     size of transmit buffer
     * @param   trigger the data-sequence, that triggers transmission
     * @param   timeout when to stop waiting for the trigger-sequence
     * @param   notTrig if set to true, start transmission at first byte
     *          that doesn't match the trigger.
     *          if set to false start transmission with first byte that matches
     *          the trigger
     *          (default: false)
     * @retval  int32_t number of sent bytes, value < 0 on failure
     */
    int32_t writeTrig(
        const void* sendBuf, size_t len, uint8_t trigger, int64_t timeout, bool notTrig = false);

    /**
     * @brief   Request data from a spi slave device.
     *          Define byte, which, once received on the miso line, triggers
     *          the start of the reception.
     *          Alternatively possible to have the function trigger once this
     *          byte is not received anymore.
     *          To get data from slave the master must generate the SPI clock and dummy
     *          data must be sent. For it recBuf will be used.
     *          Does not return until transfer is finished(blocking).
     * @param   recBuf  pointer to receive buffer
     * @param   rxLen   size of receive buffer
     * @param   trigger the data-sequence, that triggers reception
     * @param   timeout when to stop waiting for the trigger-sequence
     * @param   notTrig if set to true, start receiving at first byte
     *          that doesn't match the trigger.
     *          if set to false start transmission with first byte that matches
     *          the trigger
     *          (default: false)
     * @retval  int32_t number of received bytes, value < 0 on failure
     */
    int32_t readTrig(
        void* recBuf, size_t rxLen, uint8_t trigger, int64_t timeout, bool notTrig = false);

    /**
     * @brief   Send and receive data FULL-DUPLEX.
     *          Define byte, which, once received on the miso line, triggers
     *          the start of the transmission and reception.
     *          Alternatively possible to have the function trigger once this
     *          byte is not received anymore.
     *          With every sent byte one byte is received and put in recBuf.
     *          If recBuf is bigger than sendBuf dummy bytes will be sent to
     *          get requested data.
     *          If sendBuf is bigger than recBuf you will only get the first
     *          received bytes until recBuf is full.
     *          Does not return until transfer is finished(blocking).
     * @param   sendBuf pointer to transmit buffer
     * @param   len     size of transmit buffer
     * @param   recvBuf pointer to receive buffer
     * @param   maxLen  size of receive buffer
     * @param   trigger the data-sequence, that triggers transmission
     * @param   timeout when to stop waiting for the trigger-sequence
     * @param   notTrig if set to true, start transmission at first byte
     *          that doesn't match the trigger.
     *          if set to false start transmission with first byte that matches
     *          the trigger
     *          (default: false)
     * @retval  int32_t number of received bytes, value < 0 on failure
     */
    int32_t writeReadTrig(
        const void* sendBuf,
        size_t len,
        void* recvBuf,
        size_t maxLen,
        uint8_t trigger,
        int64_t timeout,
        bool notTrig = false);
};

}
