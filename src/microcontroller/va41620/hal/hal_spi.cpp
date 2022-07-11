#include "microcontroller/va41620/hal/hal_spi.h"

namespace RODOS {

HalSpi::HalSpi(RODOS::SpiIdx spiIdx)
    : m_context { spiIdx }
{
}

void HalSpi::init(uint32_t baudrate)
{
    m_context.init(baudrate);
}

int32_t HalSpi::write(const void* txBuffer, size_t txSize)
{
    // NOLINTNEXTLINE
    bool ok = m_context.writeRead(static_cast<const std::byte*>(txBuffer), txSize, nullptr, 0);
    return ok ? static_cast<int32_t>(txSize) : -1;
}

int32_t HalSpi::read(void* rxBuffer, size_t rxSize)
{
    // NOLINTNEXTLINE
    bool ok = m_context.writeRead(nullptr, 0, reinterpret_cast<std::byte*>(rxBuffer), rxSize);
    return ok ? static_cast<int32_t>(rxSize) : -1;
}

int32_t HalSpi::writeRead(const void* txBuffer, size_t txSize, void* rxBuffer, size_t rxSize)
{
    // NOLINTNEXTLINE
    bool ok = m_context.writeRead(
        reinterpret_cast<const std::byte*>(txBuffer), // NOLINT
        txSize,
        reinterpret_cast<std::byte*>(rxBuffer), // NOLINT
        rxSize);
    return ok ? static_cast<int32_t>(rxSize) : -1;
}

}
