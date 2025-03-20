#pragma once

#include <string>

/**
 * @class AESWrapper
 * @brief A wrapper class for AES encryption and decryption operations.
 *
 * This class provides functionalities to generate AES keys, encrypt plaintext, and decrypt ciphertext.
 */
class AESWrapper
{
public:
    static constexpr unsigned int DEFAULT_KEYLENGTH = 16; ///< Default length of the AES key.

private:
    unsigned char _key[DEFAULT_KEYLENGTH]; ///< AES key.

    /**
     * @brief Copy constructor (private to prevent copying).
     */
    AESWrapper(const AESWrapper& aes);

public:
    /**
     * @brief Generates a random AES key.
     * @param buffer Buffer to store the generated key.
     * @param length Length of the key to generate.
     * @return Pointer to the buffer containing the generated key.
     */
    static unsigned char* GenerateKey(unsigned char* buffer, unsigned int length);

    /**
     * @brief Default constructor that generates a random AES key.
     */
    AESWrapper();

    /**
     * @brief Constructs an AESWrapper object with a specified key.
     * @param key Pointer to the key data.
     * @param size Size of the key data.
     */
    AESWrapper(const unsigned char* key, unsigned int size);

    /**
     * @brief Destructor for the AESWrapper class.
     */
    ~AESWrapper();

    /**
     * @brief Gets the AES key.
     * @return Pointer to the AES key.
     */
    const unsigned char* getKey() const;

    /**
     * @brief Encrypts the given plaintext.
     * @param plain Pointer to the plaintext data.
     * @param length Length of the plaintext data.
     * @return The encrypted string.
     */
    std::string encrypt(const char* plain, unsigned int length);

    /**
     * @brief Decrypts the given ciphertext.
     * @param cipher Pointer to the ciphertext data.
     * @param length Length of the ciphertext data.
     * @return The decrypted string.
     */
    std::string decrypt(const char* cipher, unsigned int length);
};