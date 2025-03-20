#pragma once

#include <string>
#include <cryptopp/base64.h>

class Base64Wrapper
{
public:
    /**
     * @brief Encodes a given string to Base64 format.
     * 
     * @param str The input string to be encoded.
     * @return The Base64 encoded string.
     */
    static std::string encode(const std::string& str);

    /**
     * @brief Decodes a given Base64 encoded string.
     * 
     * @param str The Base64 encoded string to be decoded.
     * @return The decoded string.
     */
    static std::string decode(const std::string& str);
};