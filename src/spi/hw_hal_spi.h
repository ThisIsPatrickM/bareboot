#include "hal/hal_spi.h"
#include "peripheral_ctrl/pin_config/enums.h"
#include "peripheral_ctrl/register_maps/spi_registers.h"

#include <cstddef>

namespace RODOS {

class HW_HAL_SPI {
public:
    HW_HAL_SPI(SPI_IDX idx, GPIO_PIN sckPin, GPIO_PIN misoPin, GPIO_PIN mosiPin, GPIO_PIN nssPin);
    void init(uint32_t baudrate);

    bool writeRead(const std::byte* txBuffer, size_t txLen, std::byte* rxBuffer, size_t rxLen);

    static constexpr bool ENABLE_ROM_SPI { false }; ///< enables SPI_IDX3, which can be used for accessing the FRAM. Use with care.
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
