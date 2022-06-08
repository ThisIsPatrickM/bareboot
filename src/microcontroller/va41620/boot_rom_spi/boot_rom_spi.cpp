#include "boot_rom_spi.h"

namespace bootloader::va41620::boot_rom_spi {

void BootRomSpi::getBootRomGlobalImageMetadataOverSpi(GlobalImageMetadata& globalImageMetadata)
{
    SpiRead readMessage {};
    putAddressOffsetInto18bits(readMessage.address18bit, METADATA_OFFSET);
    uint8_t receiveBuffer[sizeof(GlobalImageMetadata) + SPI_RECEIVE_ADDRESSED_DATA_OFFSET] = { 0 };
    int32_t numberOfBytesReceived = m_halSpi.writeRead(
        &readMessage, sizeof(readMessage), &globalImageMetadata, sizeof(GlobalImageMetadata));
    if (numberOfBytesReceived <= 0) {
        return;
    }
    rodos::memcpy(&globalImageMetadata, receiveBuffer, sizeof(GlobalImageMetadata));
}

void BootRomSpi::updatePreferredImageOverSpi(size_t preferredImageIndex)
{
    if (preferredImageIndex >= PlatformParameters::NUMBER_OF_IMAGES) {
        return;
    }
    SpiWrite<sizeof(size_t)> writeMessage {};
    putAddressOffsetInto18bits(writeMessage.address18bit, METADATA_PREFERRED_IMAGE_OFFSET);
    rodos::memcpy(writeMessage.data, &preferredImageIndex, sizeof(size_t));

    enableWriting();
    m_halSpi.write(&writeMessage, sizeof(writeMessage));
    disableWriting();
}

void BootRomSpi::updateGlobalBootcounterOverSpi(uint32_t bootcounter)
{
    SpiWrite<sizeof(uint32_t)> writeMessage {};
    putAddressOffsetInto18bits(writeMessage.address18bit, METADATA_GLOBAL_BOOTCOUNTER_OFFSET);
    rodos::memcpy(writeMessage.data, &bootcounter, sizeof(uint32_t));

    enableWriting();
    m_halSpi.write(&writeMessage, sizeof(writeMessage));
    disableWriting();
}

void BootRomSpi::updateGlobalInitializedOverSpi(bool initialized)
{
    SpiWrite<sizeof(bool)> writeMessage {};
    putAddressOffsetInto18bits(writeMessage.address18bit, METADATA_GLOBAL_BOOTCOUNTER_OFFSET);
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
    putAddressOffsetInto18bits(writeMessage.address18bit, address);
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
    putAddressOffsetInto18bits(writeMessage.address18bit, address);
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
    putAddressOffsetInto18bits(writeMessage.address18bit, address);
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
    putAddressOffsetInto18bits(writeMessage.address18bit, address);
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
    putAddressOffsetInto18bits(writeMessage.address18bit, address);
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
    putAddressOffsetInto18bits(writeMessage.address18bit, address);
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

void BootRomSpi::putAddressOffsetInto18bits(
    uint8_t address[SPI_ADDRESS_SPACE_BYTES], uint32_t addressOffset)
{
    // TODO Adjust this to linter
    // TODO Maybe use more than 18 bits?
    address[0] = (addressOffset >> 16) & 0x03; // NOLINT
    address[1] = (addressOffset >> 8) & 0xff; // NOLINT
    address[2] = (addressOffset)&0xff; // NOLINT
}
}
