#include "hmac.h"

namespace bootloader::hmac {

using namespace std;
using namespace hash::sha_256;

void HMac::hmacSha256(
    const uint8_t key[],
    size_t keyLength,
    const void* data,
    size_t dataLength,
    uint8_t hmac[HMAC_DIGEST_SIZE])
{
    // Create HMAC Context and Hash Inner Pad
    HMacContext innerContext = createContextForBlockwiseHMacSha256Transformation(key, keyLength);
    uint8_t innerHash[Sha256::SHA256_DIGEST_SIZE] = { 0 };
    // Hash Message
    Sha256::sha256(
        static_cast<const uint8_t*>(data), dataLength, innerHash, &innerContext.shaContext);

    // Perform outer Hash
    Sha256Context outerContext = {};
    Sha256::sha256Transform(outerContext, innerContext.keyedOuterPad);
    // NOLINTNEXTLINE(readability-suspicious-call-argument)
    Sha256::sha256(innerHash, sizeof(innerHash), hmac, &outerContext);
}
HMacContext HMac::createContextForBlockwiseHMacSha256Transformation(
    const uint8_t key[], size_t keyLength)
{
    HMacContext context {};
    uint8_t localKey[Sha256::SHA256_BLOCK_SIZE] = { 0 };
    uint8_t keyedInnerPad[Sha256::SHA256_BLOCK_SIZE] = { 0 };

    if (keyLength > Sha256::SHA256_BLOCK_SIZE) {
        // Hash key if it is too long
        Sha256::sha256(key, keyLength, localKey);
    } else {
        RODOS::memcpy(localKey, key, keyLength);
    }

    // Store key into pads
    for (size_t i = 0; i < keyLength; i++) {
        keyedInnerPad[i] = localKey[i] ^ HMAC_INNER_PAD;
        context.keyedOuterPad[i] = localKey[i] ^ HMAC_OUTER_PAD;
    }
    // Pad remaining bytes
    for (size_t i = keyLength; i < Sha256::SHA256_BLOCK_SIZE; i++) {
        keyedInnerPad[i] = HMAC_INNER_PAD;
        context.keyedOuterPad[i] = HMAC_OUTER_PAD;
    }

    // Perform First Inner hash that does nto require Data
    Sha256::sha256Transform(context.shaContext, keyedInnerPad);
    return context;
}

void HMac::hmacSha256TransformBlock(
    HMacContext& context, const uint8_t block[hash::sha_256::Sha256::SHA256_BLOCK_SIZE])
{
    Sha256::sha256Transform(context.shaContext, block);
}

void HMac::hmacSha256FinaliseLastBlock(
    HMacContext& context,
    const uint8_t lastBlock[hash::sha_256::Sha256::SHA256_BLOCK_SIZE],
    std::size_t lastBlockLength,
    uint8_t hmac[HMAC_DIGEST_SIZE])
{
    if (lastBlockLength >= Sha256::SHA256_BLOCK_SIZE) {
        return;
    }

    // Finish Inner Hash
    uint8_t innerHash[Sha256::SHA256_DIGEST_SIZE] = { 0 };
    Sha256::sha256Finalise(context.shaContext, lastBlock, lastBlockLength, innerHash);

    // Finish outer hash
    Sha256Context outerContext = {};
    Sha256::sha256Transform(outerContext, context.keyedOuterPad);
    // NOLINTNEXTLINE(readability-suspicious-call-argument)
    Sha256::sha256(innerHash, sizeof(innerHash), hmac, &outerContext);
}

}
