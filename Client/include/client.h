//
// Created by yairm on 13/01/2025.
//

#ifndef CLIENT_H
#define CLIENT_H
#include <vector>
#include <cryptopp/config_int.h>
#include "utils.h"
#include <cstring>
#include <memory>

/**
 * @class Client
 * @brief A class representing a client with a unique ID, public key, and symmetric key.
 *
 * This class provides functionalities to store and retrieve client information,
 * such as client ID, public key, and symmetric key. It also provides methods to
 * check the existence of keys and compare clients.
 */
class Client
{
private:
    std::vector<CryptoPP::byte> _cid; ///< Client ID.
    std::vector<CryptoPP::byte> _publicKey; ///< Public key.
    std::vector<CryptoPP::byte> _symKey; ///< Symmetric key.

    bool _hasPublicKey = false; ///< Flag indicating if the public key is set.
    bool _hasSymKey = false; ///< Flag indicating if the symmetric key is set.
    bool _hasAskedForSymKey = false;

public:
    /**
     * @brief Default constructor.
     */
    Client() = default;

    /**
     * @brief Constructor that initializes the client with a client ID.
     * @param cid The client ID.
     */
    explicit Client(const std::vector<CryptoPP::byte>& cid);

    /**
     * @brief Sets the public key for the client.
     * @param publicKey The public key to set.
     */
    void setPublicKey(const std::vector<CryptoPP::byte>& publicKey);

    /**
     * @brief Sets the symmetric key for the client.
     * @param symKey The symmetric key to set.
     */
    void setSymKey(const std::vector<CryptoPP::byte>& symKey);

    /**
     * @brief Retrieves the client ID.
     * @return The client ID.
     */
    std::vector<CryptoPP::byte> getCid() const;

    /**
     * @brief Retrieves the public key.
     * @return The public key.
     */
    std::vector<CryptoPP::byte> getPublicKey() const;

    /**
     * @brief Retrieves the symmetric key.
     * @return The symmetric key.
     */
    std::vector<CryptoPP::byte> getSymKey() const;

    /**
     * @brief Checks if the public key exists.
     * @return True if the public key exists, false otherwise.
     */
    bool pubKeyExists() const;

    /**
     * @brief Checks if the symmetric key exists.
     * @return True if the symmetric key exists, false otherwise.
     */
    bool symKeyExists() const;

    /**
     * @brief Comparison operator for comparing clients.
     * @param b The client to compare with.
     * @return True if this client is less than the other client, false otherwise.
     */
    bool operator<(const Client& b) const;
};

#endif //CLIENT_H