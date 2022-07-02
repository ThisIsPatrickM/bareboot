#pragma once

#include "microcontroller/va41620/peripheral_ctrl/peripheral_defs.h"
#include "microcontroller/va41620/peripheral_ctrl/pin_config/enums.h"
#include "microcontroller/va41620/peripheral_ctrl/register_maps/spi_registers.h" // Directly needed

// TODO How is this selected?
#include "microcontroller/va41620/subtargets/peb1_100mhz/clkgen_config.h"

#include <cstddef>
#include <limits>

namespace rodos {

enum SpiIdx { //  STM32F4
    SPI_IDX0, //  not available
    SPI_IDX1, //  SPI1
    SPI_IDX2, //  SPI2
    SPI_IDX3, //   SPI3
    SPI_IDX4, //  SPI4 (STM32F429 only)
    SPI_IDX5, //  SPI5 (STM32F429 only)
    SPI_IDX6 //  SPI6 (STM32F429 only)
};

enum SpiParameterType {
    SPI_PARAMETER_BAUDRATE,
    SPI_PARAMETER_MODE, // MODE:CPOL/CPHA  0:0/0   1:0/1   2:1/0   3:1/1
    SPI_PARAMETER_MOSI_IDL_HIGH, // value 0 -> low, else high
};

enum SpiStatusType { SPI_STATUS_BAUDRATE, SPI_STATUS_MODE };

class HwHalSpi {
public:
    HwHalSpi(rodos::SpiIdx spiIdx);
    void init(uint32_t baudrate);

    bool writeRead(const std::byte* txBuffer, size_t txLen, std::byte* rxBuffer, size_t rxLen);

private:
    const rodos::SpiIdx m_idx;
    SPIStruct& m_spi;

    struct SpiPrescalers {
        uint16_t clkprescale = 0;
        uint16_t scrdv = 0;
    };

    static SpiPrescalers calculatePrescalers(int32_t baudrate);
};

}
