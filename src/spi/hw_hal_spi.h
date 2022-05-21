#pragma once

#include "src/peripheral_ctrl/pin_config/enums.h"
#include "src/peripheral_ctrl/register_maps/spi_registers.h"

#include <cstddef>

namespace RODOS {

enum SPI_IDX { //  STM32F4
    SPI_IDX0, //  not available
    SPI_IDX1, //  SPI1
    SPI_IDX2, //  SPI2
    SPI_IDX3, //   SPI3
    SPI_IDX4, //  SPI4 (STM32F429 only)
    SPI_IDX5, //  SPI5 (STM32F429 only)
    SPI_IDX6 //  SPI6 (STM32F429 only)
};

enum SPI_PARAMETER_TYPE {
    SPI_PARAMETER_BAUDRATE,
    SPI_PARAMETER_MODE, // MODE:CPOL/CPHA  0:0/0   1:0/1   2:1/0   3:1/1
    SPI_PARAMETER_MOSI_IDL_HIGH, // value 0 -> low, else high
};

enum SPI_STATUS_TYPE { SPI_STATUS_BAUDRATE, SPI_STATUS_MODE };

class HW_HAL_SPI {
public:
    HW_HAL_SPI(SPI_IDX idx, GPIO_PIN sckPin, GPIO_PIN misoPin, GPIO_PIN mosiPin, GPIO_PIN nssPin);
    void init(uint32_t baudrate);

    bool writeRead(const std::byte* txBuffer, size_t txLen, std::byte* rxBuffer, size_t rxLen);

    static constexpr bool ENABLE_ROM_SPI {
        false
    }; ///< enables SPI_IDX3, which can be used for accessing the FRAM. Use with care.
    static constexpr SPI_IDX MAX_SPI_IDX { ENABLE_ROM_SPI ? SPI_IDX3 : SPI_IDX2 };

private:
    const SPI_IDX m_idx;
    SPIStruct& m_spi;
    const pin_config::VA41620_PIN m_misoPin;
    const pin_config::VA41620_PIN m_mosiPin;
    const pin_config::VA41620_PIN m_sckPin;
    const pin_config::VA41620_PIN m_nssPin;

    void configurePin(pin_config::VA41620_PIN pin);
    pin_config::SPI_NSS_INDEX configureSlaveSelect(pin_config::VA41620_PIN pin);

    struct SpiPrescalers {
        uint16_t clkprescale;
        uint16_t scrdv;
    };

    SpiPrescalers calculatePrescalers(int32_t baudrate);
};
}
