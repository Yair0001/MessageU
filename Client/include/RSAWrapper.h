#pragma once

#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <string>

/**
 * @class RSAPublicWrapper
 * @brief A wrapper class for handling RSA public key operations.
 *
 * This class provides functionalities to store and use RSA public keys.
 * It allows for encryption using the RSA algorithm.
 *
 * @note This class uses the Crypto++ library for cryptographic operations.
 */
class RSAPublicWrapper
{
public:
    static const unsigned int KEYSIZE = 160; ///< Size of the key in bytes.
    static const unsigned int BITS = 1024;  ///< Size of the key in bits.

private:
    CryptoPP::AutoSeededRandomPool _rng; ///< Random number generator.
    CryptoPP::RSA::PublicKey _publicKey; ///< RSA public key.

    /**
     * @brief Copy constructor (private to prevent copying).
     */
    RSAPublicWrapper(const RSAPublicWrapper& rsapublic);

    /**
     * @brief Assignment operator (private to prevent copying).
     */
    RSAPublicWrapper& operator=(const RSAPublicWrapper& rsapublic);

public:
    /**
     * @brief Constructor that initializes the public key from a char array.
     * @param key Pointer to the key data.
     * @param length Length of the key data.
     */
    RSAPublicWrapper(const char* key, unsigned int length);

    /**
     * @brief Constructor that initializes the public key from a string.
     * @param key String containing the key data.
     */
    RSAPublicWrapper(const std::string& key);

    /**
     * @brief Destructor.
     */
    ~RSAPublicWrapper();

    /**
     * @brief Retrieves the public key as a string.
     * @return The public key as a string.
     */
    std::string getPublicKey() const;

    /**
     * @brief Retrieves the public key and copies it to the provided buffer.
     * @param keyout Buffer to copy the key to.
     * @param length Length of the buffer.
     * @return Pointer to the buffer containing the key.
     */
    char* getPublicKey(char* keyout, unsigned int length) const;

    /**
     * @brief Encrypts a plaintext string.
     * @param plain The plaintext string to encrypt.
     * @return The encrypted string.
     */
    std::string encrypt(const std::string& plain);

    /**
     * @brief Encrypts a plaintext char array.
     * @param plain Pointer to the plaintext data.
     * @param length Length of the plaintext data.
     * @return The encrypted string.
     */
    std::string encrypt(const char* plain, unsigned int length);
};

/**
 * @class RSAPrivateWrapper
 * @brief A wrapper class for handling RSA private key operations.
 *
 * This class provides functionalities to store and use RSA private keys.
 * It allows for decryption using the RSA algorithm.
 *
 * @note This class uses the Crypto++ library for cryptographic operations.
 */
class RSAPrivateWrapper
{
public:
    static const unsigned int BITS = 1024;  ///< Size of the key in bits.

private:
    CryptoPP::AutoSeededRandomPool _rng; ///< Random number generator.
    CryptoPP::RSA::PrivateKey _privateKey; ///< RSA private key.

    /**
     * @brief Copy constructor (private to prevent copying).
     */
    RSAPrivateWrapper(const RSAPrivateWrapper& rsaprivate);

    /**
     * @brief Assignment operator (private to prevent copying).
     */
    RSAPrivateWrapper& operator=(const RSAPrivateWrapper& rsaprivate);

public:
    /**
     * @brief Default constructor.
     */
    RSAPrivateWrapper();

    /**
     * @brief Constructor that initializes the private key from a char array.
     * @param key Pointer to the key data.
     * @param length Length of the key data.
     */
    RSAPrivateWrapper(const char* key, unsigned int length);

    /**
     * @brief Constructor that initializes the private key from a string.
     * @param key String containing the key data.
     */
    RSAPrivateWrapper(const std::string& key);

    /**
     * @brief Destructor.
     */
    ~RSAPrivateWrapper();

    /**
     * @brief Retrieves the private key as a string.
     * @return The private key as a string.
     */
    std::string getPrivateKey() const;

    /**
     * @brief Retrieves the private key and copies it to the provided buffer.
     * @param keyout Buffer to copy the key to.
     * @param length Length of the buffer.
     * @return Pointer to the buffer containing the key.
     */
    char* getPrivateKey(char* keyout, unsigned int length) const;

    /**
     * @brief Retrieves the public key as a string.
     * @return The public key as a string.
     */
    std::string getPublicKey() const;

    /**
     * @brief Retrieves the public key and copies it to the provided buffer.
     * @param keyout Buffer to copy the key to.
     * @param length Length of the buffer.
     * @return Pointer to the buffer containing the key.
     */
    char* getPublicKey(char* keyout, unsigned int length) const;

    /**
     * @brief Decrypts an encrypted string.
     * @param cipher The encrypted string to decrypt.
     * @return The decrypted string.
     */
    std::string decrypt(const std::string& cipher);

    /**
     * @brief Decrypts an encrypted char array.
     * @param cipher Pointer to the encrypted data.
     * @param length Length of the encrypted data.
     * @return The decrypted string.
     */
    std::string decrypt(const char* cipher, unsigned int length);
};