#include "boot_rom_spi.h"

namespace bootloader::va41620::boot_rom_spi {

void BootRomSpi::getBootRomGlobalImageMetadataOverSpi(GlobalImageMetadata& globalImageMetadata)
{
    // TODO The same code failed on bootmanager. Failed because of memcopy, copying 1 byte each, but
    // only uint16 or bigger is possible. TODO Fix in both implementaitons
    SpiRead readMessage {};
    putAddressOffsetIntoMessage(readMessage.address, METADATA_OFFSET);
    uint8_t receiveBuffer[sizeof(GlobalImageMetadata) + SPI_RECEIVE_ADDRESSED_DATA_OFFSET] = { 0 };
    int32_t numberOfBytesReceived = m_halSpi.writeRead(
        &readMessage, sizeof(readMessage), &globalImageMetadata, sizeof(GlobalImageMetadata));
    if (numberOfBytesReceived <= 0) {
        return;
    }

    bootloader::memcpy::va41620UnsignedMemcpy(
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
    putAddressOffsetIntoMessage(writeMessage.address, METADATA_PREFERRED_IMAGE_OFFSET);
    bootloader::memcpy::va41620UnsignedMemcpy(
        writeMessage.data, &preferredImageIndex, sizeof(size_t));

    enableWriting();
    m_halSpi.write(&writeMessage, sizeof(writeMessage));
    disableWriting();
}

void BootRomSpi::updateCurrentImageOverSpi(size_t currentImageIndex)
{
    if (currentImageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return;
    }
    SpiWrite<sizeof(size_t)> writeMessage = {};
    putAddressOffsetIntoMessage(writeMessage.address, METADATA_CURRENT_IMAGE_OFFSET);
    bootloader::memcpy::va41620UnsignedMemcpy(
        writeMessage.data, &currentImageIndex, sizeof(size_t));

    enableWriting();
    m_halSpi.write(&writeMessage, sizeof(writeMessage));
    disableWriting();
}

void BootRomSpi::updateGlobalBootcounterOverSpi(uint32_t bootcounter)
{
    SpiWrite<sizeof(uint32_t)> writeMessage {};
    putAddressOffsetIntoMessage(writeMessage.address, METADATA_GLOBAL_BOOTCOUNTER_OFFSET);
    bootloader::memcpy::va41620UnsignedMemcpy(writeMessage.data, &bootcounter, sizeof(uint32_t));

    enableWriting();
    m_halSpi.write(&writeMessage, sizeof(writeMessage));
    disableWriting();
}

void BootRomSpi::updateGlobalInitializedOverSpi(bool initialized)
{
    SpiWrite<sizeof(bool)> writeMessage {};
    putAddressOffsetIntoMessage(writeMessage.address, METADATA_GLOBAL_BOOTCOUNTER_OFFSET);
    bootloader::memcpy::va41620UnsignedMemcpy(writeMessage.data, &initialized, sizeof(bool));

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
    putAddressOffsetIntoMessage(writeMessage.address, address);
    bootloader::memcpy::va41620UnsignedMemcpy(writeMessage.data, &version, sizeof(version));
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
    putAddressOffsetIntoMessage(writeMessage.address, address);
    bootloader::memcpy::va41620UnsignedMemcpy(writeMessage.data, &crc, sizeof(crc));
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
    bootloader::memcpy::va41620UnsignedMemcpy(writeMessage.data, &bootcounter, sizeof(bootcounter));
    enableWriting();
    m_halSpi.write(&writeMessage, sizeof(writeMessage));
    disableWriting();
}

void BootRomSpi::updateImageCompletionStatusOverSpi(
    CompletionStatus completionStatus, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return;
    }
    SpiWrite<sizeof(completionStatus)> writeMessage {};
    uint32_t address = METADATA_IMAGES_OFFSET + imageIndex * sizeof(ImageMetadata) +
                       offsetof(ImageMetadata, completionStatus);
    putAddressOffsetIntoMessage(writeMessage.address, address);
    bootloader::memcpy::va41620UnsignedMemcpy(
        writeMessage.data, &completionStatus, sizeof(completionStatus));
    enableWriting();
    m_halSpi.write(&writeMessage, sizeof(writeMessage));
    disableWriting();
}

void BootRomSpi::updateImageProtectionStatusOverSpi(
    ProtectionStatus protectionStatus, size_t imageIndex)
{
    if (imageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return;
    }
    SpiWrite<sizeof(protectionStatus)> writeMessage {};
    uint32_t address = METADATA_IMAGES_OFFSET + imageIndex * sizeof(ImageMetadata) +
                       offsetof(ImageMetadata, protectionStatus);
    putAddressOffsetIntoMessage(writeMessage.address, address);
    bootloader::memcpy::va41620UnsignedMemcpy(
        writeMessage.data, &protectionStatus, sizeof(protectionStatus));
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
    putAddressOffsetIntoMessage(writeMessage.address, address);
    bootloader::memcpy::va41620UnsignedMemcpy(writeMessage.data, &length, sizeof(length));
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
    putAddressOffsetIntoMessage(writeMessage.address, address);
    bootloader::memcpy::va41620UnsignedMemcpy(writeMessage.data, &imageBegin, sizeof(imageBegin));
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
    const auto* dataPtr = static_cast<const uint8_t*>(data);

    while (remainingLength > 0) {
        int32_t fragmentSize = remainingLength > BUFFER_SIZE ? BUFFER_SIZE : remainingLength;

        // Write Fragment
        SpiWrite<BUFFER_SIZE> spiWrite {};
        putAddressOffsetIntoMessage(spiWrite.address, imagePointer);
        bootloader::memcpy::va41620UnsignedMemcpy(spiWrite.data, dataPtr, fragmentSize);
        enableWriting();
        m_halSpi.write(&spiWrite, fragmentSize + SPI_RECEIVE_ADDRESSED_DATA_OFFSET);
        disableWriting();

        // Update parameters
        remainingLength -= fragmentSize;
        dataPtr += fragmentSize; // NOLINT
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
        putAddressOffsetIntoMessage(spiRead.address, srcImagePointer);
        m_halSpi.writeRead(
            &spiRead, sizeof(spiRead), buffer, fragmentSize + SPI_RECEIVE_ADDRESSED_DATA_OFFSET);

        // Write Fragment
        SpiWrite<BUFFER_SIZE> spiWrite {};
        putAddressOffsetIntoMessage(spiWrite.address, dstImagePointer);
        bootloader::memcpy::va41620UnsignedMemcpy(
            spiWrite.data, &buffer[SPI_RECEIVE_ADDRESSED_DATA_OFFSET], fragmentSize);
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
        putAddressOffsetIntoMessage(spiRead.address, imagePointer);
        m_halSpi.writeRead(
            &spiRead, fragmentSize + SPI_RECEIVE_ADDRESSED_DATA_OFFSET, buffer, sizeof(buffer));

        bootloader::memcpy::va41620UnsignedMemcpy(
            destinationPtr, &buffer[SPI_RECEIVE_ADDRESSED_DATA_OFFSET], fragmentSize);

        // Update parameters
        remainingLength -= fragmentSize;
        imagePointer += fragmentSize;
        destinationPtr += fragmentSize; // NOLINT
    }
}

}
