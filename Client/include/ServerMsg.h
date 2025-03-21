//
// Created by yairm on 19/01/2025.
//

#ifndef SERVERMSG_H
#define SERVERMSG_H
#include <vector>
#include <cryptopp/trap.h>
#include "client.h"
#include <unordered_map>
#include <boost/bimap.hpp>
#include "AESWrapper.h"
#include "RSAWrapper.h"

/**
 * @class ServerMsg
 * @brief A class for handling server messages.
 *
 * This class provides functionalities to parse and handle messages received from the server.
 */
class ServerMsg {
private:
    std::vector<CryptoPP::byte> _version; ///< Version information.
    std::vector<CryptoPP::byte> _code; ///< Command code.
    std::vector<CryptoPP::byte> _payloadSize; ///< Size of the payload.
    std::vector<CryptoPP::byte> _payload; ///< Payload data.

public:
    /**
     * @brief Constructor that initializes the ServerMsg with a message.
     * @param msg The message to initialize with.
     */
    explicit ServerMsg(const std::vector<CryptoPP::byte>& msg);

    /**
     * @brief Gets the command code.
     * @return The command code.
     */
    std::vector<CryptoPP::byte> getCode() const;

    /**
     * @brief Gets the payload size as a vector of bytes.
     * @return The payload size as a vector of bytes.
     */
    std::vector<CryptoPP::byte> getPayloadSizeVec() const;

    /**
     * @brief Gets the payload size as an integer.
     * @return The payload size as an integer.
     */
    int getPayloadSizeInt() const;

    /**
     * @brief Gets the payload data.
     * @return The payload data.
     */
    std::vector<CryptoPP::byte> getPayload() const;

    /**
     * @brief Checks if the command code is valid.
     * @param code The command code to check.
     * @return True if the command code is valid, false otherwise.
     */
    static bool isValidCode(int code);

    /**
     * @brief Checks if there are errors in the server message.
     * @param ans The server message to check.
     * @return True if there are errors, false otherwise.
     */
    static bool errorsExist(const ServerMsg& ans);

    /**
     * @brief Prints the error message from the server message.
     * @param ans The server message containing the error.
     */
    static void printError(const ServerMsg& ans);

    /**
     * @brief Prints the list of clients.
     * @param clients The list of clients.
     * @param clientList The bimap of client names and Client objects.
     */
    static void printClientsList(const std::vector<std::vector<CryptoPP::byte>>& clients, boost::bimap<std::string, Client>& clientList);

    /**
     * @brief Prints the message content.
     * @param msg The message content.
     * @param privateKey The private key for decryption.
     * @param clientList The bimap of client names and Client objects.
     * @param hasAskedForSymKey User asked for sym key
     */
    static void printMsg(const std::vector<CryptoPP::byte>& msg, RSAPrivateWrapper& privateKey, boost::bimap<std::string, Client>& clientList, bool& hasAskedForSymKey);
};

#endif //SERVERMSG_H