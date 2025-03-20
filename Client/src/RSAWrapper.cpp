#include "RSAWrapper.h"

/**
 * @brief Constructs an RSAPublicWrapper object with a specified key.
 * @param key Pointer to the key data.
 * @param length Length of the key data.
 */
RSAPublicWrapper::RSAPublicWrapper(const char* key, unsigned int length)
{
    CryptoPP::StringSource ss(reinterpret_cast<const CryptoPP::byte*>(key), length, true);
    _publicKey.Load(ss);
}

/**
 * @brief Constructs an RSAPublicWrapper object with a specified key.
 * @param key String containing the key data.
 */
RSAPublicWrapper::RSAPublicWrapper(const std::string& key)
{
    CryptoPP::StringSource ss(key, true);
    _publicKey.Load(ss);
}

/**
 * @brief Destructor for the RSAPublicWrapper class.
 */
RSAPublicWrapper::~RSAPublicWrapper()
{
}

/**
 * @brief Retrieves the public key as a string.
 * @return The public key as a string.
 */
std::string RSAPublicWrapper::getPublicKey() const
{
    std::string key;
    CryptoPP::StringSink ss(key);
    _publicKey.Save(ss);
    return key;
}

/**
 * @brief Retrieves the public key and copies it to the provided buffer.
 * @param keyout Buffer to copy the key to.
 * @param length Length of the buffer.
 * @return Pointer to the buffer containing the key.
 */
char* RSAPublicWrapper::getPublicKey(char* keyout, unsigned int length) const
{
    CryptoPP::ArraySink as(reinterpret_cast<CryptoPP::byte*>(keyout), length);
    _publicKey.Save(as);
    return keyout;
}

/**
 * @brief Encrypts a plaintext string.
 * @param plain The plaintext string to encrypt.
 * @return The encrypted string.
 */
std::string RSAPublicWrapper::encrypt(const std::string& plain)
{
    std::string cipher;
    CryptoPP::RSAES_OAEP_SHA_Encryptor e(_publicKey);
    CryptoPP::StringSource ss(plain, true, new CryptoPP::PK_EncryptorFilter(_rng, e, new CryptoPP::StringSink(cipher)));
    return cipher;
}

/**
 * @brief Encrypts a plaintext char array.
 * @param plain Pointer to the plaintext data.
 * @param length Length of the plaintext data.
 * @return The encrypted string.
 */
std::string RSAPublicWrapper::encrypt(const char* plain, unsigned int length)
{
    std::string cipher;
    CryptoPP::RSAES_OAEP_SHA_Encryptor e(_publicKey);
    CryptoPP::StringSource ss(reinterpret_cast<const CryptoPP::byte*>(plain), length, true, new CryptoPP::PK_EncryptorFilter(_rng, e, new CryptoPP::StringSink(cipher)));
    return cipher;
}

/**
 * @brief Default constructor that generates a new RSA private key.
 */
RSAPrivateWrapper::RSAPrivateWrapper()
{
    _privateKey.Initialize(_rng, BITS);
}

/**
 * @brief Constructs an RSAPrivateWrapper object with a specified key.
 * @param key Pointer to the key data.
 * @param length Length of the key data.
 */
RSAPrivateWrapper::RSAPrivateWrapper(const char* key, unsigned int length)
{
    CryptoPP::StringSource ss(reinterpret_cast<const CryptoPP::byte*>(key), length, true);
    _privateKey.Load(ss);
}

/**
 * @brief Constructs an RSAPrivateWrapper object with a specified key.
 * @param key String containing the key data.
 */
RSAPrivateWrapper::RSAPrivateWrapper(const std::string& key)
{
    CryptoPP::StringSource ss(key, true);
    _privateKey.Load(ss);
}

/**
 * @brief Destructor for the RSAPrivateWrapper class.
 */
RSAPrivateWrapper::~RSAPrivateWrapper()
{
}

/**
 * @brief Retrieves the private key as a string.
 * @return The private key as a string.
 */
std::string RSAPrivateWrapper::getPrivateKey() const
{
    std::string key;
    CryptoPP::StringSink ss(key);
    _privateKey.Save(ss);
    return key;
}

/**
 * @brief Retrieves the private key and copies it to the provided buffer.
 * @param keyout Buffer to copy the key to.
 * @param length Length of the buffer.
 * @return Pointer to the buffer containing the key.
 */
char* RSAPrivateWrapper::getPrivateKey(char* keyout, unsigned int length) const
{
    CryptoPP::ArraySink as(reinterpret_cast<CryptoPP::byte*>(keyout), length);
    _privateKey.Save(as);
    return keyout;
}

/**
 * @brief Retrieves the public key as a string.
 * @return The public key as a string.
 */
std::string RSAPrivateWrapper::getPublicKey() const
{
    CryptoPP::RSAFunction publicKey(_privateKey);
    std::string key;
    CryptoPP::StringSink ss(key);
    publicKey.Save(ss);
    return key;
}

/**
 * @brief Retrieves the public key and copies it to the provided buffer.
 * @param keyout Buffer to copy the key to.
 * @param length Length of the buffer.
 * @return Pointer to the buffer containing the key.
 */
char* RSAPrivateWrapper::getPublicKey(char* keyout, unsigned int length) const
{
    CryptoPP::RSAFunction publicKey(_privateKey);
    CryptoPP::ArraySink as(reinterpret_cast<CryptoPP::byte*>(keyout), length);
    publicKey.Save(as);
    return keyout;
}

/**
 * @brief Decrypts an encrypted string.
 * @param cipher The encrypted string to decrypt.
 * @return The decrypted string.
 */
std::string RSAPrivateWrapper::decrypt(const std::string& cipher)
{
    std::string decrypted;
    CryptoPP::RSAES_OAEP_SHA_Decryptor d(_privateKey);
    CryptoPP::StringSource ss_cipher(cipher, true, new CryptoPP::PK_DecryptorFilter(_rng, d, new CryptoPP::StringSink(decrypted)));
    return decrypted;
}

/**
 * @brief Decrypts an encrypted char array.
 * @param cipher Pointer to the encrypted data.
 * @param length Length of the encrypted data.
 * @return The decrypted string.
 */
std::string RSAPrivateWrapper::decrypt(const char* cipher, unsigned int length)
{
    std::string decrypted;
    CryptoPP::RSAES_OAEP_SHA_Decryptor d(_privateKey);
    CryptoPP::StringSource ss_cipher(reinterpret_cast<const CryptoPP::byte*>(cipher), length, true, new CryptoPP::PK_DecryptorFilter(_rng, d, new CryptoPP::StringSink(decrypted)));
    return decrypted;
}