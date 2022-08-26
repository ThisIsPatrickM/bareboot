

#include "metadata.h"
#include "microcontroller/metadata_interface.h"
#include "platform_parameters.h"
#include <gtest/gtest.h>

using namespace bootloader;
using namespace testing;

class MetadataInterfaceTestSuite : public ::testing::Test {
public:
    static constexpr size_t EXPECTED_NUMBER_OF_IMAGES = 7;
    static constexpr uint32_t EXPECTED_LENGTH = 74;
    static constexpr size_t IMAGE_INDEX = 1;
    static constexpr size_t IMAGE_INDEX_OUT_OF_BOUNDS = PlatformParameters::NUMBER_OF_IMAGES;
    static constexpr size_t EXPECTED_PREFFERED_IMAGE = 4;
    static constexpr size_t EXPECTED_CURRENT_IMAGE = 3;
    static constexpr size_t EXPECTED_BOOTCOUNT = 99;
    static constexpr size_t EXPECTED_VERSION = 98;
    static constexpr uint32_t EXPECTED_CRC = 0xdeadbeef;
    static constexpr CompletionStatus EXPECTED_COMPLETION_STATUS = CompletionStatus::COMPLETE;
    static constexpr ProtectionStatus EXPECTED_PROTECTION_STATUS = ProtectionStatus::PROTECTED;
    static constexpr uint8_t SAMPLE_DATA[] = { 1, 2, 3, 4, 5, 6 };

    MetadataInterface meta { reinterpret_cast<GlobalImageMetadata*>(__bootloader__) }; // NOLINT
    const GlobalImageMetadata* globalMetadata = meta.getGlobalImageMetadata();
};

TEST_F(MetadataInterfaceTestSuite, PlatformParameterTest)
{
    EXPECT_EQ(PlatformParameters::NUMBER_OF_IMAGES, EXPECTED_NUMBER_OF_IMAGES);
}

TEST_F(MetadataInterfaceTestSuite, UpdatePrefferedImage)
{
    meta.updatePreferredImage(EXPECTED_PREFFERED_IMAGE);
    EXPECT_EQ(globalMetadata->preferredImage, EXPECTED_PREFFERED_IMAGE);

    meta.updatePreferredImage(IMAGE_INDEX_OUT_OF_BOUNDS);
    EXPECT_EQ(globalMetadata->preferredImage, EXPECTED_PREFFERED_IMAGE);
}

TEST_F(MetadataInterfaceTestSuite, UpdateCurrentImage)
{
    meta.updateCurrentImage(EXPECTED_CURRENT_IMAGE);
    EXPECT_EQ(globalMetadata->currentImage, EXPECTED_CURRENT_IMAGE);

    meta.updateCurrentImage(IMAGE_INDEX_OUT_OF_BOUNDS);
    EXPECT_EQ(globalMetadata->currentImage, EXPECTED_CURRENT_IMAGE);
}

TEST_F(MetadataInterfaceTestSuite, UpdateGlobalBootCounter)
{
    meta.updateGlobalBootcounter(EXPECTED_BOOTCOUNT);
    EXPECT_EQ(globalMetadata->globalBootcounter, EXPECTED_BOOTCOUNT);
}

TEST_F(MetadataInterfaceTestSuite, UpdateImageVersion)
{
    meta.updateImageVersion(EXPECTED_VERSION, IMAGE_INDEX);
    EXPECT_EQ(globalMetadata->images[IMAGE_INDEX].version, EXPECTED_VERSION);

    meta.updateImageVersion(EXPECTED_VERSION + 1, IMAGE_INDEX_OUT_OF_BOUNDS);
    EXPECT_EQ(globalMetadata->images[IMAGE_INDEX].version, EXPECTED_VERSION);
}

TEST_F(MetadataInterfaceTestSuite, UpdateImageCrc)
{
    meta.updateImageCrc(EXPECTED_CRC, IMAGE_INDEX);
    EXPECT_EQ(globalMetadata->images[IMAGE_INDEX].crc, EXPECTED_CRC);

    meta.updateImageCrc(EXPECTED_CRC + 1, IMAGE_INDEX_OUT_OF_BOUNDS);
    EXPECT_EQ(globalMetadata->images[IMAGE_INDEX].crc, EXPECTED_CRC);
}

TEST_F(MetadataInterfaceTestSuite, UpdateImageBootcount)
{
    meta.updateImageBootcounter(EXPECTED_BOOTCOUNT, IMAGE_INDEX);
    EXPECT_EQ(globalMetadata->images[IMAGE_INDEX].bootcounter, EXPECTED_BOOTCOUNT);

    meta.updateImageBootcounter(EXPECTED_BOOTCOUNT + 1, IMAGE_INDEX_OUT_OF_BOUNDS);
    EXPECT_EQ(globalMetadata->images[IMAGE_INDEX].bootcounter, EXPECTED_BOOTCOUNT);
}

TEST_F(MetadataInterfaceTestSuite, UpdateImageCompletionStatus)
{
    meta.updateImageCompletionStatus(EXPECTED_COMPLETION_STATUS, IMAGE_INDEX);
    EXPECT_EQ(globalMetadata->images[IMAGE_INDEX].completionStatus, EXPECTED_COMPLETION_STATUS);

    meta.updateImageCompletionStatus(CompletionStatus::INCOMPLETE, IMAGE_INDEX_OUT_OF_BOUNDS);
    EXPECT_EQ(globalMetadata->images[IMAGE_INDEX].completionStatus, EXPECTED_COMPLETION_STATUS);
}

TEST_F(MetadataInterfaceTestSuite, UpdateImageProtectionStatus)
{
    meta.updateImageProtectionStatus(EXPECTED_PROTECTION_STATUS, IMAGE_INDEX);
    EXPECT_EQ(globalMetadata->images[IMAGE_INDEX].protectionStatus, EXPECTED_PROTECTION_STATUS);

    meta.updateImageProtectionStatus(ProtectionStatus::UNPROTECTED, IMAGE_INDEX_OUT_OF_BOUNDS);
    EXPECT_EQ(globalMetadata->images[IMAGE_INDEX].protectionStatus, EXPECTED_PROTECTION_STATUS);
}

TEST_F(MetadataInterfaceTestSuite, UpdateImageLength)
{
    meta.updateImageLength(EXPECTED_LENGTH, IMAGE_INDEX);
    EXPECT_EQ(globalMetadata->images[IMAGE_INDEX].length, EXPECTED_LENGTH);

    meta.updateImageLength(EXPECTED_LENGTH + 1, IMAGE_INDEX_OUT_OF_BOUNDS);
    EXPECT_EQ(globalMetadata->images[IMAGE_INDEX].length, EXPECTED_LENGTH);

    meta.updateImageLength(PlatformParameters::MAX_IMAGE_LENGTH, IMAGE_INDEX);
    EXPECT_EQ(globalMetadata->images[IMAGE_INDEX].length, EXPECTED_LENGTH);
}

TEST_F(MetadataInterfaceTestSuite, GetNumberOfImages)
{
    EXPECT_EQ(meta.getNumberOfImages(), PlatformParameters::NUMBER_OF_IMAGES);
}

TEST_F(MetadataInterfaceTestSuite, GetMaxImageLength)
{
    EXPECT_EQ(meta.getMaxImageLength(), PlatformParameters::MAX_IMAGE_LENGTH);
}

TEST_F(MetadataInterfaceTestSuite, InitDoesNotBreak)
{
    meta.init();
}

TEST_F(MetadataInterfaceTestSuite, UpdateImage)
{
    // TODO This only writes to boot ROM
    // TODO Maybe only testing for error handling?
    meta.updateImage(SAMPLE_DATA, sizeof(SAMPLE_DATA), IMAGE_INDEX, 0);
    meta.updateImage(
        SAMPLE_DATA, sizeof(SAMPLE_DATA), IMAGE_INDEX, PlatformParameters::MAX_IMAGE_LENGTH);
    meta.updateImage(SAMPLE_DATA, sizeof(SAMPLE_DATA), IMAGE_INDEX_OUT_OF_BOUNDS, 0);
}

TEST_F(MetadataInterfaceTestSuite, CopyImage)
{
    // TODO Mock Bootrom SPI? / HAL SPI and check result
    meta.copyImage(IMAGE_INDEX, IMAGE_INDEX + 1);
    meta.copyImage(IMAGE_INDEX_OUT_OF_BOUNDS, IMAGE_INDEX + 1);
    meta.copyImage(IMAGE_INDEX, IMAGE_INDEX_OUT_OF_BOUNDS);
    meta.copyImage(IMAGE_INDEX_OUT_OF_BOUNDS, IMAGE_INDEX_OUT_OF_BOUNDS);
}

TEST_F(MetadataInterfaceTestSuite, LoadImage)
{
    // TODO Mock Bootrom SPI? / HAL SPI and check result
    // meta.loadImage(IMAGE_INDEX);
    // TODO Add further tests. Currently not possible due to differetn handling of labels
    meta.loadImage(IMAGE_INDEX_OUT_OF_BOUNDS);
}

TEST_F(MetadataInterfaceTestSuite, VerifyChecksum)
{
    // TODO Add further tests.
    meta.updateImageLength(2, IMAGE_INDEX);
    EXPECT_FALSE(meta.verifyChecksum(IMAGE_INDEX));
    meta.updateImageLength(20, IMAGE_INDEX);
    EXPECT_FALSE(meta.verifyChecksum(IMAGE_INDEX));

    EXPECT_FALSE(meta.verifyChecksum(IMAGE_INDEX_OUT_OF_BOUNDS));
}
