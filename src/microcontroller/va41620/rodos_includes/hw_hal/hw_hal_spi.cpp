#include "hw_hal_spi.h"

namespace RODOS {

HwHalSpi::HwHalSpi(SpiIdx spiIdx)
    : m_idx { spiIdx }
    , m_spi { SPIS[spiIdx] } // NOLINT
{
}

void HwHalSpi::init(uint32_t baudrate)
{
    using namespace SPI_CTRL0;
    using namespace SPI_CTRL1;
    using namespace SPI_CLKPRESCALE;
    SYSCONFIG_PERIPHERAL_BITBAND->SPI_ENABLE[m_idx].write(1);

    constexpr uint8_t WORD_SIZE_BITS { 8 };

    pin_config::SpiNssIndex nssIndex { pin_config::SpiNssIndex::SS0 };

    SpiPrescalers prescalers { calculatePrescalers(static_cast<int32_t>(baudrate)) };
    m_spi.CLKPRESCALE.write(VALUE(prescalers.clkprescale));
    m_spi.CTRL0.write(SIZE(WORD_SIZE_BITS - 1), SCRDV(prescalers.scrdv));
    m_spi.CTRL1.write(SS(static_cast<uint32_t>(nssIndex)), BLOCKMODE(1), BMSTALL(1), ENABLE(1));
}

HwHalSpi::SpiPrescalers HwHalSpi::calculatePrescalers(int32_t baudrate)
{
    // see Programmer's Guide section 11.4.5 for baudrate calculation
    constexpr int32_t CLOCK_FREQUENCY_NO_PRESCALER { GLOBAL_CLOCK_SETUP.getSysclkFrequency() / 4 };
    constexpr uint8_t MAX_SCRDV_VALUE { 255 };
    constexpr uint8_t MAX_CLKPRESCALE_VALUE { 127 };
    uint16_t bestClkprescaleMatch { 0 };
    uint16_t bestScrdvMatch { 0 };
    uint32_t bestAbsoluteBaudrateError { std::numeric_limits<uint32_t>::max() };
    for (uint16_t scrdv = 0; scrdv <= MAX_SCRDV_VALUE; scrdv++) {
        uint16_t maxUsefulClkprescale { static_cast<uint16_t>(
            (scrdv + 1) > MAX_CLKPRESCALE_VALUE ? MAX_CLKPRESCALE_VALUE : scrdv + 1) };
        for (uint16_t clkprescale = 1; clkprescale <= maxUsefulClkprescale; clkprescale++) {
            int32_t baudrateResult { static_cast<int32_t>(
                CLOCK_FREQUENCY_NO_PRESCALER / ((scrdv + 1) * clkprescale)) };
            int32_t baudrateError { baudrate - baudrateResult };
            uint32_t absoluteBaudrateError { static_cast<uint32_t>(
                baudrateError > 0 ? baudrateError : -baudrateError) };
            if (absoluteBaudrateError < bestAbsoluteBaudrateError) {
                bestClkprescaleMatch = clkprescale;
                bestScrdvMatch = scrdv;
                bestAbsoluteBaudrateError = absoluteBaudrateError;
            }
        }
    }

    return { bestClkprescaleMatch, bestScrdvMatch };
}

bool HwHalSpi::writeRead(const std::byte* txBuffer, size_t txLen, std::byte* rxBuffer, size_t rxLen)
{
    using namespace SPI_STATUS;
    using namespace SPI_DATA;
    using namespace SPI_FIFO_CLR;
    if ((txBuffer == nullptr && txLen != 0) || (rxBuffer == nullptr && rxLen != 0)) {
        return false;
    }
    constexpr std::byte DUMMY_BYTE { 0xFF };

    m_spi.FIFO_CLR.write(TXFIFO(1), RXFIFO(1));

    const size_t bytesToTransmit = (txLen > rxLen) ? txLen : rxLen;
    size_t transmittedBytes { 0 };
    size_t receivedBytes { 0 };
    while (transmittedBytes < bytesToTransmit || receivedBytes < rxLen) {
        RegisterCopy status = m_spi.STATUS;
        if (status.read<TNF>() && transmittedBytes < bytesToTransmit) { // NOLINT
            std::byte byteToWrite { transmittedBytes < txLen ? txBuffer[transmittedBytes] // NOLINT
                                                             : DUMMY_BYTE };
            transmittedBytes++;
            bool isLastByte { transmittedBytes == bytesToTransmit };
            m_spi.DATA.write(VALUE(byteToWrite), BMSTOP(isLastByte ? 1 : 0));
        }
        if (status.read<RNE>() && receivedBytes < rxLen) { // NOLINT
            rxBuffer[receivedBytes] = static_cast<std::byte>(m_spi.DATA.read()); // NOLINT
            receivedBytes++;
        }
    }
    while (m_spi.STATUS.read<BUSY>()) // NOLINT
        ;
    return true;
}

}
