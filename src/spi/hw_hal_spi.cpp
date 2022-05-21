#include "hw_hal_spi.h"
#include "clkgen_config.h"
#include "peripheral_ctrl/peripheral_defs.h"
#include "peripheral_ctrl/pin_config/pin_config.h"
#include "peripheral_ctrl/pin_config/pinmap.h"

#include <limits>

namespace RODOS {

HW_HAL_SPI::HW_HAL_SPI(SPI_IDX idx, GPIO_PIN sckPin, GPIO_PIN misoPin, GPIO_PIN mosiPin, GPIO_PIN nssPin)
: m_idx { idx }
, m_spi { spis[idx] }
, m_misoPin { misoPin } 
, m_mosiPin { mosiPin }
, m_sckPin { sckPin }
, m_nssPin { nssPin } 
{}

void HW_HAL_SPI::init(uint32_t baudrate) {
    using namespace SPI_CTRL0;
    using namespace SPI_CTRL1;
    using namespace SPI_CLKPRESCALE;
    sysconfigPeripheralBitband->SPI_ENABLE[m_idx].write(1);

    constexpr uint8_t WORD_SIZE_BITS { 8 };

    configurePin(m_misoPin);
    configurePin(m_mosiPin);
    configurePin(m_sckPin);
    pin_config::SPI_NSS_INDEX nssIndex { configureSlaveSelect(m_nssPin) };

    SpiPrescalers prescalers { calculatePrescalers(static_cast<int32_t>(baudrate)) };
    m_spi.CLKPRESCALE.write(VALUE(prescalers.clkprescale));
    m_spi.CTRL0.write(SIZE(WORD_SIZE_BITS-1), SCRDV(prescalers.scrdv));

    if(nssIndex == pin_config::SPI_NSS_INDEX::INVALID){
        m_spi.CTRL1.write(SOD(1), BLOCKMODE(1), BMSTALL(1), ENABLE(1));
    }else{
        m_spi.CTRL1.write(SS(static_cast<uint32_t>(nssIndex)), BLOCKMODE(1), BMSTALL(1), ENABLE(1));
    }
}

HW_HAL_SPI::SpiPrescalers HW_HAL_SPI::calculatePrescalers(int32_t baudrate){
    //see Programmer's Guide section 11.4.5 for baudrate calculation
    constexpr int32_t CLOCK_FREQUENCY_NO_PRESCALER { globalClockSetup.getSysclkFrequency()/4 };
    constexpr uint8_t MAX_SCRDV_VALUE { 255 };
    constexpr uint8_t MAX_CLKPRESCALE_VALUE { 127 };
    uint16_t bestClkprescaleMatch { 0 };
    uint16_t bestScrdvMatch { 0 };
    uint32_t bestAbsoluteBaudrateError { std::numeric_limits<uint32_t>::max() };
    for(uint16_t scrdv = 0; scrdv<=MAX_SCRDV_VALUE; scrdv++){
        uint16_t maxUsefulClkprescale { static_cast<uint16_t>((scrdv+1)>MAX_CLKPRESCALE_VALUE ? MAX_CLKPRESCALE_VALUE : scrdv+1) };
        for(uint16_t clkprescale = 1; clkprescale<=maxUsefulClkprescale; clkprescale++){
            int32_t baudrateResult { static_cast<int32_t>(CLOCK_FREQUENCY_NO_PRESCALER/((scrdv+1)*clkprescale)) };
            int32_t baudrateError { baudrate-baudrateResult };
            uint32_t absoluteBaudrateError { static_cast<uint32_t>(baudrateError>0 ? baudrateError : -baudrateError) };
            if(absoluteBaudrateError < bestAbsoluteBaudrateError){
                bestClkprescaleMatch = clkprescale;
                bestScrdvMatch = scrdv;
                bestAbsoluteBaudrateError = absoluteBaudrateError;
            }
        }
    }

    return { bestClkprescaleMatch, bestScrdvMatch };
}

void HW_HAL_SPI::configurePin(pin_config::VA41620_PIN pin){
    if(pin.isValid()){
        pin_config::configurePin(pin, pin_config::SPI_PIN_MAPS.getFunction(m_idx, pin));
    }
}

pin_config::SPI_NSS_INDEX HW_HAL_SPI::configureSlaveSelect(pin_config::VA41620_PIN pin){
    if(pin.isValid()){
        pin_config::NssPinFunction nssFunction { pin_config::SPI_NSS_MAPS.getFunction(m_idx, pin) };
        pin_config::configurePin(pin, nssFunction.function);
        return nssFunction.nssIndex;
    }else{
        constexpr pin_config::SPI_NSS_INDEX ROM_NSS_PIN_INDEX = pin_config::SPI_NSS_INDEX::SS0;
        if(m_idx == SPI_IDX3){
            return ROM_NSS_PIN_INDEX;
        }else{
            return pin_config::SPI_NSS_INDEX::INVALID;
        }
    }
}

bool HW_HAL_SPI::writeRead(const std::byte* txBuffer, size_t txSize, std::byte* rxBuffer, size_t rxSize) {
    using namespace SPI_STATUS;
    using namespace SPI_DATA;
    using namespace SPI_FIFO_CLR;
    if((txBuffer==nullptr&&txSize!=0) || (rxBuffer==nullptr&&rxSize!=0)){        
        return false;
    }
    constexpr std::byte DUMMY_BYTE { 0xFF };

    m_spi.FIFO_CLR.write(TXFIFO(1), RXFIFO(1));

    const size_t bytesToTransmit = (txSize>rxSize) ? txSize : rxSize;
    size_t transmittedBytes { 0 };
    size_t receivedBytes { 0 };
    while(transmittedBytes<bytesToTransmit || receivedBytes<rxSize) {
        RegisterCopy status = m_spi.STATUS;
        if(status.read<TNF>() && transmittedBytes<bytesToTransmit) {
            std::byte byteToWrite { transmittedBytes<txSize ? txBuffer[transmittedBytes] : DUMMY_BYTE };
            transmittedBytes++;
            bool isLastByte { transmittedBytes == bytesToTransmit };
            m_spi.DATA.write(VALUE(byteToWrite), BMSTOP(isLastByte ? 1 : 0) );
        }
        if(status.read<RNE>() && receivedBytes<rxSize) {
            rxBuffer[receivedBytes] = static_cast<std::byte>(m_spi.DATA.read());
            receivedBytes++;
        }
    }
    while(m_spi.STATUS.read<BUSY>());
    return true;
}

}
