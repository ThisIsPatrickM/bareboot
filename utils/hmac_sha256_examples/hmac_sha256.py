import hmac
import hashlib
from message_samples import MESSAGES, formatToConstexprValue


if __name__ == '__main__':

    key = b"ABCDEF"

    for m in MESSAGES:
        h = hmac.new(key, m, hashlib.sha256)
        print("HMAC for message ", m)
        print(h.hexdigest())
        formatToConstexprValue(h.digest())
        print("")
