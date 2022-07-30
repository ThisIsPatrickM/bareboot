SAMPLE_MESSAGE = b"ABCDEFGHIJKL"
LONG_MESSAGE = b"hello worldhello worldhello worldhello worldhello worldhello worldhello worldhello worldhello worldhello worldhello worldhello worldhello worldhello worldhello world\0"
BLOCK_SIZED_MESSAGE = b'oneblockoneblockoneblockoneblockoneblockoneblockoneblockonebloc\0'
UNPADDED_MESSAGE = b'oneblockoneblockoneblockoneblockoneblockoneblockoneblo\0'
PADDED_MESSAGE = b'oneblockoneblockoneblockoneblockoneblockoneblockonebloc\0'

MESSAGES = [SAMPLE_MESSAGE, LONG_MESSAGE,
            BLOCK_SIZED_MESSAGE, UNPADDED_MESSAGE, PADDED_MESSAGE]


def formatToConstexprValue(digest):
    formatted = "{ "
    for byte in digest:
        formatted += hex(byte) + ", "

    formattedList = list(formatted)
    formattedList[len(formatted) - 2] = "}"
    print("".join(formattedList))
    print("")
