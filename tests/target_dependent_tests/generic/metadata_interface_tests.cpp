

#include "metadata.h"
#include "microcontroller/metadata_interface.h"
#include "platform_parameters.h"
#include <gtest/gtest.h>

using namespace bootloader;
using namespace testing;

class MetadataInterfaceTestSuite : public ::testing::Test {
public:
    static constexpr size_t EXPECTED_NUMBER_OF_IMAGES = 4;
    static constexpr uint32_t EXPECTED_LENGTH = 74;
    static constexpr size_t IMAGE_INDEX = 1;
    static constexpr size_t IMAGE_INDEX_OUT_OF_BOUNDS = PlatformParameters::NUMBER_OF_IMAGES;
    static constexpr size_t EXPECTED_PREFFERED_IMAGE = 2;
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

TEST_F(MetadataInterfaceTestSuite, UpdateImageBootcount)
{
    meta.updateImageBootcounter(EXPECTED_BOOTCOUNT, IMAGE_INDEX);
    EXPECT_EQ(globalMetadata->images[IMAGE_INDEX].bootcounter, EXPECTED_BOOTCOUNT);

    meta.updateImageBootcounter(EXPECTED_BOOTCOUNT + 1, IMAGE_INDEX_OUT_OF_BOUNDS);
    EXPECT_EQ(globalMetadata->images[IMAGE_INDEX].bootcounter, EXPECTED_BOOTCOUNT);
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

TEST_F(MetadataInterfaceTestSuite, LoadImageDoesNotBreak)
{
    meta.loadImage(IMAGE_INDEX);
    meta.loadImage(IMAGE_INDEX_OUT_OF_BOUNDS);
}
