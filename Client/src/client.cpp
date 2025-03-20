#include "client.h"

/**
 * @brief Constructs a Client object with a specified client ID.
 * @param cid The client ID.
 */
Client::Client(const std::vector<CryptoPP::byte>& cid){
    _cid = cid;
    _publicKey.resize(PUBLIC_KEY_SIZE);
    _symKey.resize(SYM_KEY_SIZE);
}

/**
 * @brief Sets the public key for the client.
 * @param publicKey The public key to set.
 */
void Client::setPublicKey(const std::vector<CryptoPP::byte>& publicKey){
    _publicKey = publicKey;
    _hasPublicKey = true;
}

/**
 * @brief Sets the symmetric key for the client.
 * @param symKey The symmetric key to set.
 */
void Client::setSymKey(const std::vector<CryptoPP::byte>& symKey){
    _symKey = symKey;
}

/**
 * @brief Retrieves the client ID.
 * @return The client ID.
 */
std::vector<CryptoPP::byte> Client::getCid() const{
    return _cid;
}

/**
 * @brief Retrieves the public key.
 * @return The public key.
 */
std::vector<CryptoPP::byte> Client::getPublicKey() const{
    return _publicKey;
}

/**
 * @brief Retrieves the symmetric key.
 * @return The symmetric key.
 */
std::vector<CryptoPP::byte> Client::getSymKey() const{
    return _symKey;
}

/**
 * @brief Checks if the public key exists.
 * @return True if the public key exists, false otherwise.
 */
bool Client::pubKeyExists() const{
    return _hasPublicKey;
}

/**
 * @brief Checks if the symmetric key exists.
 * @return True if the symmetric key exists, false otherwise.
 */
bool Client::symKeyExists() const{
    return _hasSymKey;
}

/**
 * @brief Comparison operator for comparing clients.
 * @param b The client to compare with.
 * @return True if this client is less than the other client, false otherwise.
 */
bool Client::operator<(const Client& b) const
{
    return bytesToString(getCid()) < bytesToString(b.getCid());
}