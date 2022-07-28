#include "hmac.h"

namespace bootloader::hmac {

using namespace std;

void HMAC::hmacSha256(
    [[maybe_unused]] uint8_t key[HMAC_KEY_LENGTH],
    [[maybe_unused]] size_t keyLength,
    [[maybe_unused]] const void* data,
    [[maybe_unused]] size_t dataLength,
    [[maybe_unused]] uint8_t hmac[HMAC_DIGEST_LENGTH])
{
}

}
