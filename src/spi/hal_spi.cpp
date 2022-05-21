// #include "misc-rodos-funcs.h"
#include "hw_hal_spi.h"

#include <new>

namespace RODOS {

HAL_SPI::HAL_SPI(SPI_IDX idx, GPIO_PIN sckPin, GPIO_PIN misoPin, GPIO_PIN mosiPin, GPIO_PIN nssPin)
    : context { xmallocAndTrapOnFailure<HW_HAL_SPI>() }
{
    if (idx > HW_HAL_SPI::MAX_SPI_IDX) {
        __builtin_trap();
    }
    new (context) HW_HAL_SPI(idx, sckPin, misoPin, mosiPin, nssPin);
}

HAL_SPI::HAL_SPI(SPI_IDX idx)
    : HAL_SPI(idx, GPIO_INVALID, GPIO_INVALID, GPIO_INVALID, GPIO_INVALID)
{
}

int32_t HAL_SPI::init(uint32_t baudrate, bool slave, bool tiMode)
{
    if (slave || tiMode) {
        RODOS_ERROR("SPI slave or TI mode currently not supported");
        return -1;
    }
    context->init(baudrate);
    return 0;
}

bool HAL_SPI::isWriteFinished()
{
    return true;
}

void HAL_SPI::reset()
{
    RODOS_ERROR("SPI reset not supported yet!");
}

int32_t HAL_SPI::config(SPI_PARAMETER_TYPE, int32_t)
{
    RODOS_ERROR("SPI config not supported yet!");
    return -1;
}

int32_t HAL_SPI::status(SPI_STATUS_TYPE)
{
    RODOS_ERROR("SPI status not supported yet!");
    return -1;
}

bool HAL_SPI::isReadFinished()
{
    return true;
}

int32_t HAL_SPI::write(const void* txBuffer, size_t txSize)
{
    bool ok = context->writeRead(static_cast<const std::byte*>(txBuffer), txSize, nullptr, 0);
    return ok ? static_cast<int32_t>(txSize) : -1;
}

int32_t HAL_SPI::read(void* rxBuffer, size_t rxSize)
{
    bool ok = context->writeRead(nullptr, 0, reinterpret_cast<std::byte*>(rxBuffer), rxSize);
    return ok ? static_cast<int32_t>(rxSize) : -1;
}

int32_t HAL_SPI::writeRead(const void* txBuffer, size_t txSize, void* rxBuffer, size_t rxSize)
{
    bool ok = context->writeRead(
        reinterpret_cast<const std::byte*>(txBuffer),
        txSize,
        reinterpret_cast<std::byte*>(rxBuffer),
        rxSize);
    return ok ? static_cast<int32_t>(rxSize) : -1;
}

int32_t HAL_SPI::writeTrig(const void*, size_t, uint8_t, int64_t, bool)
{
    RODOS_ERROR("SPI trig methods not implemented");
    return -1;
}

int32_t HAL_SPI::readTrig(void*, size_t, uint8_t, int64_t, bool)
{
    RODOS_ERROR("SPI trig methods not implemented");
    return -1;
}

int32_t HAL_SPI::writeReadTrig(const void*, size_t, void*, size_t, uint8_t, int64_t, bool)
{
    RODOS_ERROR("SPI trig methods not implemented");
    return -1;
}

}
