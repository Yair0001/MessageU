#include "Base64Wrapper.h"

/**
 * @brief Encodes a given string to Base64 format.
 * @param str The input string to be encoded.
 * @return The Base64 encoded string.
 */
std::string Base64Wrapper::encode(const std::string& str)
{
    std::string encoded;
    CryptoPP::StringSource ss(str, true,
        new CryptoPP::Base64Encoder(
            new CryptoPP::StringSink(encoded)
        ) // Base64Encoder
    ); // StringSource

    return encoded;
}

/**
 * @brief Decodes a given Base64 encoded string.
 * @param str The Base64 encoded string to be decoded.
 * @return The decoded string.
 */
std::string Base64Wrapper::decode(const std::string& str)
{
    std::string decoded;
    CryptoPP::StringSource ss(str, true,
        new CryptoPP::Base64Decoder(
            new CryptoPP::StringSink(decoded)
        ) // Base64Decoder
    ); // StringSource

    return decoded;
}