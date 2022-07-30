import hashlib
from message_samples import MESSAGES, formatToConstexprValue


if __name__ == '__main__':

    for m in MESSAGES:

        sha = hashlib.sha256()
        sha.update(m)
        print("Digest of ", m)
        print(sha.hexdigest())
        formatToConstexprValue(sha.digest())
