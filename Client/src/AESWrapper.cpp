#include "AESWrapper.h"

#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>

#include <stdexcept>
#include <immintrin.h>	// _rdrand32_step

/**
 * @brief Generates a random AES key.
 * @param buffer Buffer to store the generated key.
 * @param length Length of the key to generate.
 * @return Pointer to the buffer containing the generated key.
 */
unsigned char* AESWrapper::GenerateKey(unsigned char* buffer, unsigned int length)
{
    for (size_t i = 0; i < length; i += sizeof(unsigned int))
        _rdrand32_step(reinterpret_cast<unsigned int*>(&buffer[i]));
    return buffer;
}

/**
 * @brief Default constructor that generates a random AES key.
 */
AESWrapper::AESWrapper()
{
    GenerateKey(_key, DEFAULT_KEYLENGTH);
}

/**
 * @brief Constructs an AESWrapper object with a specified key.
 * @param key Pointer to the key data.
 * @param length Length of the key data.
 */
AESWrapper::AESWrapper(const unsigned char* key, unsigned int length)
{
    if (length != DEFAULT_KEYLENGTH)
        throw std::length_error("key length must be 16 bytes");
    CryptoPP::memcpy_s(_key, DEFAULT_KEYLENGTH, key, length);
}

/**
 * @brief Destructor for the AESWrapper class.
 */
AESWrapper::~AESWrapper()
{
}

/**
 * @brief Gets the AES key.
 * @return Pointer to the AES key.
 */
const unsigned char* AESWrapper::getKey() const 
{ 
    return _key; 
}

/**
 * @brief Encrypts the given plaintext.
 * @param plain Pointer to the plaintext data.
 * @param length Length of the plaintext data.
 * @return The encrypted string.
 */
std::string AESWrapper::encrypt(const char* plain, unsigned int length)
{
    CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE] = { 0 };	// for practical use iv should never be a fixed value!

    CryptoPP::AES::Encryption aesEncryption(_key, DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);

    std::string cipher;
    CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(cipher));
    stfEncryptor.Put(reinterpret_cast<const CryptoPP::byte*>(plain), length);
    stfEncryptor.MessageEnd();

    return cipher;
}

/**
 * @brief Decrypts the given ciphertext.
 * @param cipher Pointer to the ciphertext data.
 * @param length Length of the ciphertext data.
 * @return The decrypted string.
 */
std::string AESWrapper::decrypt(const char* cipher, unsigned int length)
{
    CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE] = { 0 };	// for practical use iv should never be a fixed value!

    CryptoPP::AES::Decryption aesDecryption(_key, DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

    std::string decrypted;
    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(decrypted));
    stfDecryptor.Put(reinterpret_cast<const CryptoPP::byte*>(cipher), length);
    stfDecryptor.MessageEnd();

    return decrypted;
}