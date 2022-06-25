#include "boot_rom_spi.h"

namespace bootloader::va41620::boot_rom_spi {

void BootRomSpi::getBootRomGlobalImageMetadataOverSpi(GlobalImageMetadata& globalImageMetadata)
{
    // TODO The same code failed on bootmanager
    SpiRead readMessage {};
    putAddressOffsetIntoMessage(readMessage.address18bit, METADATA_OFFSET);
    uint8_t receiveBuffer[sizeof(GlobalImageMetadata) + SPI_RECEIVE_ADDRESSED_DATA_OFFSET] = { 0 };
    int32_t numberOfBytesReceived = m_halSpi.writeRead(
        &readMessage, sizeof(readMessage), &globalImageMetadata, sizeof(GlobalImageMetadata));
    if (numberOfBytesReceived <= 0) {
        return;
    }
    rodos::memcpy(
        &globalImageMetadata,
        &receiveBuffer[SPI_RECEIVE_ADDRESSED_DATA_OFFSET],
        sizeof(GlobalImageMetadata));
}

void BootRomSpi::updatePreferredImageOverSpi(size_t preferredImageIndex)
{
    if (preferredImageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return;
    }
    SpiWrite<sizeof(size_t)> writeMessage {};
    putAddressOffsetIntoMessage(writeMessage.address18bit, METADATA_PREFERRED_IMAGE_OFFSET);
    rodos::memcpy(writeMessage.data, &preferredImageIndex, sizeof(size_t));

    enableWriting();
    m_halSpi.write(&writeMessage, sizeof(writeMessage));
    disableWriting();
}

void BootRomSpi::updateGlobalBootcounterOverSpi(uint32_t bootcounter)
{
    SpiWrite<sizeof(uint32_t)> writeMessage {};
    putAddressOffsetIntoMessage(writeMessage.address18bit, METADATA_GLOBAL_BOOTCOUNTER_OFFSET);
    rodos::memcpy(writeMessage.data, &bootcounter, sizeof(uint32_t));

    enableWriting();
    m_halSpi.write(&writeMessage, sizeof(writeMessage));
    disableWriting();
}

void BootRomSpi::updateGlobalInitializedOverSpi(bool initialized)
{
    SpiWrite<sizeof(bool)> writeMessage {};
    putAddressOffsetIntoMessage(writeMessage.address18bit, METADATA_GLOBAL_BOOTCOUNTER_OFFSET);
    rodos::memcpy(writeMessage.data, &initialized, sizeof(bool));

    enableWriting();
    m_halSpi.write(&writeMessage, sizeof(writeMessage));
    disableWriting();
}

void BootRomSpi::updateImageVersionOverSpi(uint32_t version, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return;
    }
    SpiWrite<sizeof(uint32_t)> writeMessage {};
    uint32_t address = METADATA_IMAGES_OFFSET + imageIndex * sizeof(ImageMetadata) +
                       offsetof(ImageMetadata, version);
    putAddressOffsetIntoMessage(writeMessage.address18bit, address);
    rodos::memcpy(writeMessage.data, &version, sizeof(version));
    enableWriting();
    m_halSpi.write(&writeMessage, sizeof(writeMessage));
    disableWriting();
}

void BootRomSpi::updateImageCrcOverSpi(uint32_t crc, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return;
    }
    SpiWrite<sizeof(uint32_t)> writeMessage {};
    uint32_t address =
        METADATA_IMAGES_OFFSET + imageIndex * sizeof(ImageMetadata) + offsetof(ImageMetadata, crc);
    putAddressOffsetIntoMessage(writeMessage.address18bit, address);
    rodos::memcpy(writeMessage.data, &crc, sizeof(crc));
    enableWriting();
    m_halSpi.write(&writeMessage, sizeof(writeMessage));
    disableWriting();
}

void BootRomSpi::updateImageCompleteOverSpi(bool complete, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return;
    }
    SpiWrite<sizeof(uint32_t)> writeMessage {};
    uint32_t address = METADATA_IMAGES_OFFSET + imageIndex * sizeof(ImageMetadata) +
                       offsetof(ImageMetadata, complete);
    putAddressOffsetIntoMessage(writeMessage.address18bit, address);
    rodos::memcpy(writeMessage.data, &complete, sizeof(complete));
    enableWriting();
    m_halSpi.write(&writeMessage, sizeof(writeMessage));
    disableWriting();
}

void BootRomSpi::updateImageAlwaysKeepOverSpi(bool alwaysKeep, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return;
    }
    SpiWrite<sizeof(uint32_t)> writeMessage {};
    uint32_t address = METADATA_IMAGES_OFFSET + imageIndex * sizeof(ImageMetadata) +
                       offsetof(ImageMetadata, alwaysKeep);
    putAddressOffsetIntoMessage(writeMessage.address18bit, address);
    rodos::memcpy(writeMessage.data, &alwaysKeep, sizeof(alwaysKeep));
    enableWriting();
    m_halSpi.write(&writeMessage, sizeof(writeMessage));
    disableWriting();
}

void BootRomSpi::updateImageLengthOverSpi(uint32_t length, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return;
    }
    SpiWrite<sizeof(uint32_t)> writeMessage {};
    uint32_t address = METADATA_IMAGES_OFFSET + imageIndex * sizeof(ImageMetadata) +
                       offsetof(ImageMetadata, length);
    putAddressOffsetIntoMessage(writeMessage.address18bit, address);
    rodos::memcpy(writeMessage.data, &length, sizeof(length));
    enableWriting();
    m_halSpi.write(&writeMessage, sizeof(writeMessage));
    disableWriting();
}

void BootRomSpi::updateImageBeginOverSpi(void* imageBegin, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return;
    }
    SpiWrite<sizeof(void*)> writeMessage {};
    uint32_t address = METADATA_IMAGES_OFFSET + imageIndex * sizeof(ImageMetadata) +
                       offsetof(ImageMetadata, imageBegin);
    putAddressOffsetIntoMessage(writeMessage.address18bit, address);
    rodos::memcpy(writeMessage.data, &imageBegin, sizeof(imageBegin));
    enableWriting();
    m_halSpi.write(&writeMessage, sizeof(writeMessage));
    disableWriting();
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
    // TODO Adjust this to linter
    // address[0] = (addressOffset >> 16) & 0x03; // NOLINT
    address[0] = (addressOffset >> 16) & 0xFF; // NOLINT
    address[1] = (addressOffset >> 8) & 0xFF; // NOLINT
    address[2] = (addressOffset)&0xFF; // NOLINT
}

void BootRomSpi::updateImage(const void* data, int32_t length, uintptr_t imagePointer)
{
    // Writes data to SPI Address
    int32_t remainingLength = length;

    while (remainingLength > 0) {
        int32_t fragmentSize = remainingLength > BUFFER_SIZE ? BUFFER_SIZE : remainingLength;

        // Write Fragment
        SpiWrite<BUFFER_SIZE> spiWrite {};
        putAddressOffsetIntoMessage(spiWrite.address18bit, imagePointer);
        rodos::memcpy(spiWrite.data, data, fragmentSize);
        enableWriting();
        m_halSpi.write(&spiWrite, fragmentSize + SPI_RECEIVE_ADDRESSED_DATA_OFFSET);
        disableWriting();

        // Update parameters
        remainingLength -= fragmentSize;
        imagePointer += fragmentSize;
    }
}

// TODO Order Function calls

void BootRomSpi::copyImage(uintptr_t srcImagePointer, int32_t length, uintptr_t dstImagePointer)
{
    int32_t remainingLength = length;

    while (remainingLength > 0) {
        int32_t fragmentSize = remainingLength > BUFFER_SIZE ? BUFFER_SIZE : remainingLength;
        uint8_t buffer[BUFFER_SIZE + SPI_RECEIVE_ADDRESSED_DATA_OFFSET] = { 0 };
        // Read Fragment
        SpiRead spiRead {};
        putAddressOffsetIntoMessage(spiRead.address18bit, srcImagePointer);
        m_halSpi.writeRead(&spiRead, sizeof(spiRead), buffer, fragmentSize);

        // Write Fragment
        SpiWrite<BUFFER_SIZE> spiWrite {};
        putAddressOffsetIntoMessage(spiWrite.address18bit, dstImagePointer);
        rodos::memcpy(spiWrite.data, &buffer[SPI_RECEIVE_ADDRESSED_DATA_OFFSET], fragmentSize);
        enableWriting();
        m_halSpi.write(&spiWrite, fragmentSize + SPI_RECEIVE_ADDRESSED_DATA_OFFSET);
        disableWriting();

        // Update parameters
        remainingLength -= fragmentSize;
        srcImagePointer += fragmentSize;
        dstImagePointer += fragmentSize;
    }
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
        putAddressOffsetIntoMessage(spiRead.address18bit, imagePointer);
        m_halSpi.writeRead(
            &spiRead, fragmentSize + SPI_RECEIVE_ADDRESSED_DATA_OFFSET, buffer, sizeof(buffer));

        rodos::memcpy(destinationPtr, &buffer[SPI_RECEIVE_ADDRESSED_DATA_OFFSET], fragmentSize);

        // Update parameters
        remainingLength -= fragmentSize;
        imagePointer += fragmentSize;
    }
}

}
