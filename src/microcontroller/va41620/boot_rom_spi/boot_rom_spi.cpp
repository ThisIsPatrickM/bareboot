#include "boot_rom_spi.h"

namespace bootloader::va41620::boot_rom_spi {

void BootRomSpi::getBootRomGlobalImageMetadataOverSpi(GlobalImageMetadata& globalImageMetadata)
{
    SpiRead readMessage {};
    putAddressOffsetIntoMessage(readMessage.address, METADATA_OFFSET);
    uint8_t receiveBuffer[sizeof(GlobalImageMetadata) + SPI_RECEIVE_ADDRESSED_DATA_OFFSET] = { 0 };
    int32_t numberOfBytesReceived = m_halSpi.writeRead(
        &readMessage, sizeof(readMessage), &globalImageMetadata, sizeof(GlobalImageMetadata));
    if (numberOfBytesReceived <= 0) {
        return;
    }

    // Custom Memcopy required here, because assigning 1-byte values in Code ROM is not possible for
    // Vorago
    memcpy::va41620UnsignedMemcpy(
        &globalImageMetadata,
        &receiveBuffer[SPI_RECEIVE_ADDRESSED_DATA_OFFSET],
        sizeof(GlobalImageMetadata));
}

void BootRomSpi::updateCurrentImageOverSpi(size_t currentImageIndex)
{
    if (currentImageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return;
    }
    SpiWrite<sizeof(size_t)> writeMessage = {};
    putAddressOffsetIntoMessage(writeMessage.address, METADATA_CURRENT_IMAGE_OFFSET);
    RODOS::memcpy(writeMessage.data, &currentImageIndex, sizeof(size_t));

    enableWriting();
    m_halSpi.write(&writeMessage, sizeof(writeMessage));
    disableWriting();
}

void BootRomSpi::updateGlobalBootcounterOverSpi(uint32_t bootcounter)
{
    SpiWrite<sizeof(uint32_t)> writeMessage {};
    putAddressOffsetIntoMessage(writeMessage.address, METADATA_GLOBAL_BOOTCOUNTER_OFFSET);
    RODOS::memcpy(writeMessage.data, &bootcounter, sizeof(uint32_t));

    enableWriting();
    m_halSpi.write(&writeMessage, sizeof(writeMessage));
    disableWriting();
}

void BootRomSpi::updateImageBootcounterOverSpi(uint32_t bootcounter, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return;
    }
    SpiWrite<sizeof(uint32_t)> writeMessage {};
    uint32_t address = METADATA_IMAGES_OFFSET + imageIndex * sizeof(ImageMetadata) +
                       offsetof(ImageMetadata, bootcounter);
    putAddressOffsetIntoMessage(writeMessage.address, address);
    RODOS::memcpy(writeMessage.data, &bootcounter, sizeof(bootcounter));

    enableWriting();
    m_halSpi.write(&writeMessage, sizeof(writeMessage));
    disableWriting();
}

uint8_t* BootRomSpi::getDataOverSpi(uintptr_t address, uint32_t length, uint8_t* receiveBuffer)
{
    SpiRead readMessage {};
    putAddressOffsetIntoMessage(readMessage.address, static_cast<uint32_t>(address));

    m_halSpi.writeRead(
        &readMessage,
        sizeof(readMessage),
        receiveBuffer,
        length + SPI_RECEIVE_ADDRESSED_DATA_OFFSET);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    return &receiveBuffer[SPI_RECEIVE_ADDRESSED_DATA_OFFSET];
}

void BootRomSpi::init()
{
    m_halSpi.init(SPI_FREQUENCY);
}

void BootRomSpi::enableWriting()
{
    SpiWriteStatusRegister spiWriteStatusRegister {};
    spiWriteStatusRegister.statusRegister = SPI_STATUS_REGISTER_WRITE_ALL;
    m_halSpi.write(&spiWriteStatusRegister, sizeof(spiWriteStatusRegister));

    SpiWriteEnable spiWriteEnable {};
    m_halSpi.write(&spiWriteEnable, sizeof(spiWriteEnable));
}

void BootRomSpi::disableWriting()
{
    SpiResetWriteEnable spiResetWriteEnable {};
    m_halSpi.write(&spiResetWriteEnable, sizeof(spiResetWriteEnable));
}

void BootRomSpi::putAddressOffsetIntoMessage(
    uint8_t address[SPI_ADDRESS_SPACE_BYTES], uint32_t addressOffset)
{
    address[0] = (addressOffset >> 16) & 0xFF; // NOLINT
    address[1] = (addressOffset >> 8) & 0xFF; // NOLINT
    address[2] = (addressOffset)&0xFF; // NOLINT
}

void BootRomSpi::loadImage(void* destination, int32_t length, uintptr_t imagePointer)
{
    // Writes data from SPI Address to destination
    int32_t remainingLength = length;
    auto* destinationPtr = static_cast<uint8_t*>(destination);

    while (remainingLength > 0) {
        int32_t fragmentSize = remainingLength > BUFFER_SIZE ? BUFFER_SIZE : remainingLength;
        uint8_t buffer[BUFFER_SIZE + SPI_RECEIVE_ADDRESSED_DATA_OFFSET] = { 0 };

        // Write Fragment
        SpiRead spiRead {};
        putAddressOffsetIntoMessage(spiRead.address, imagePointer);
        m_halSpi.writeRead(
            &spiRead, fragmentSize + SPI_RECEIVE_ADDRESSED_DATA_OFFSET, buffer, sizeof(buffer));

        // Custom Memcpy required here!
        bootloader::memcpy::va41620UnsignedMemcpy(
            destinationPtr, &buffer[SPI_RECEIVE_ADDRESSED_DATA_OFFSET], fragmentSize);

        // Update parameters
        remainingLength -= fragmentSize;
        imagePointer += fragmentSize;
        destinationPtr += fragmentSize; // NOLINT
    }
}

}
