import hmac
import hashlib


def formatToConstexprValue(digest):
    formatted = "{ "
    for byte in digest:
        formatted += hex(byte) + ", "

    formattedList = list(formatted)
    formattedList[len(formatted) - 2] = "}"
    print("".join(formattedList))
    print("")


if __name__ == '__main__':
    SAMPLE_MESSAGE = b"ABCDEFGHIJKL"
    LONG_MESSAGE = b"hello worldhello worldhello worldhello worldhello worldhello worldhello worldhello worldhello worldhello worldhello worldhello worldhello worldhello worldhello world\0"
    BLOCK_SIZED_MESSAGE = b'oneblockoneblockoneblockoneblockoneblockoneblockoneblockonebloc\0'
    UNPADDED_MESSAGE = b'oneblockoneblockoneblockoneblockoneblockoneblockoneblo\0'
    PADDED_MESSAGE = b'oneblockoneblockoneblockoneblockoneblockoneblockonebloc\0'

    messages = [SAMPLE_MESSAGE, LONG_MESSAGE,
                BLOCK_SIZED_MESSAGE, UNPADDED_MESSAGE, PADDED_MESSAGE]
    key = b"ABCDEF"

    for m in messages:
        h = hmac.new(key, m, hashlib.sha256)
        print("HMAC for message ", m)
        print(h.hexdigest())
        formatToConstexprValue(h.digest())
        print("")
