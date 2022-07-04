class Va41620Config:

    # Size of Datatypes to calculate Offset.
    POINTER_SIZE = 4
    SIZE_T_SIZE = 4
    UINT32_T_SIZE = 4
    COMPLETE_ALWAYS_KEEP_SIZE = 4
    BOOTLOADER_SIZE = 4 * 1024
    BYTE_ORDER = 'little'

    METADATA_LABEL_OFFSET = 0x350
    FIRST_IMAGE_METADATA_OFFSET = METADATA_LABEL_OFFSET + \
        1 * UINT32_T_SIZE + 2 * SIZE_T_SIZE
    FIRST_CRC_OFFSET = FIRST_IMAGE_METADATA_OFFSET + 1 * UINT32_T_SIZE
    FIRST_LENGTH_OFFSET = FIRST_IMAGE_METADATA_OFFSET + \
        5 * UINT32_T_SIZE + 1 * POINTER_SIZE
    FIRST_COMPLETE_OFFSET = FIRST_IMAGE_METADATA_OFFSET + \
        6 * UINT32_T_SIZE + 1 * POINTER_SIZE
    SIZE_OF_IMAGE_METADATA = 6 * UINT32_T_SIZE + 1 * \
        POINTER_SIZE + 1 * COMPLETE_ALWAYS_KEEP_SIZE
    IMAGE_BEGIN_ADDRESSES = [4096, 68608, 133120, 197632]

    MAX_BINARY_IMAGE_SIZE = IMAGE_BEGIN_ADDRESSES[1] - IMAGE_BEGIN_ADDRESSES[0]


CONFIG_MAP = {
    "va41620": Va41620Config}