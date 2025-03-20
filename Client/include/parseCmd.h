#ifndef PARSECMD_H
#define PARSECMD_H

#include <cryptopp/cryptlib.h>
#include <vector>
#include <RSAWrapper.h>
#include "utils.h"
#include <fstream>
#include "ServerHandler.h"
#include "ServerMsg.h"
#include <iostream>
#include "Base64Wrapper.h"
#include "client.h"
#include <unordered_map>
#include <boost/bimap.hpp>

/**
 * @class ClientCmd
 * @brief A class for handling client commands and interactions with the server.
 *
 * This class provides functionalities to parse and execute various client commands,
 * such as registering a user, retrieving the client list, getting public keys, and sending messages.
 */
class ClientCmd {
private:
    boost::bimap<std::string, Client> _clientList; ///< Bimap to store client list with string keys and Client objects.

    ServerHandler _serverHandler; ///< Handler for server communication.

    std::vector<CryptoPP::byte> _cid; ///< Client ID.
    std::vector<CryptoPP::byte> _version; ///< Version information.
    std::vector<CryptoPP::byte> _code; ///< Command code.
    std::vector<CryptoPP::byte> _payloadSize; ///< Size of the payload.
    std::vector<CryptoPP::byte> _payload; ///< Payload data.
    std::vector<CryptoPP::byte> _userName; ///< User name.
    std::vector<CryptoPP::byte> _msgType; ///< Message type.
    std::vector<CryptoPP::byte> _contentSize; ///< Size of the message content.
    std::vector<CryptoPP::byte> _msgContent; ///< Message content.
    std::vector<CryptoPP::byte> _otherCid; ///< Other client ID.

    std::vector<CryptoPP::byte> _publicKey; ///< Public key.
    std::vector<CryptoPP::byte> _privateKey; ///< Private key.

    bool _gotClientsList;

public:
    /**
     * @brief Constructor that initializes the ClientCmd with a ServerHandler.
     * @param serverHandler Reference to a ServerHandler object.
     */
    explicit ClientCmd(ServerHandler& serverHandler);

    /**
     * @brief Parses a command string and returns the corresponding byte vector.
     * @param command The command string to parse.
     * @return A vector of bytes representing the parsed command.
     */
    std::vector<CryptoPP::byte> parseCommand(const std::string& command, bool gottenClientsList);

    /**
     * @brief Registers a user and returns the corresponding byte vector.
     * @return A vector of bytes representing the registration command.
     */
    std::vector<CryptoPP::byte> registerUser();

    /**
     * @brief Retrieves the list of clients and returns the corresponding byte vector.
     * @return A vector of bytes representing the clients list command.
     */
    std::vector<CryptoPP::byte> clientsList();

    /**
     * @brief Retrieves the public key of a client ID and returns the corresponding byte vector.
     * @return A vector of bytes representing the get public key command.
     */
    std::vector<CryptoPP::byte> getPublicKeyOfCid();

    /**
     * @brief Retrieves the waiting messages and returns the corresponding byte vector.
     * @return A vector of bytes representing the waiting messages command.
     */
    std::vector<CryptoPP::byte> waitingMsgs();

    /**
     * @brief Sends a message and returns the corresponding byte vector.
     * @return A vector of bytes representing the send message command.
     */
    std::vector<CryptoPP::byte> sendMsg();

    /**
     * @brief Checks if a name exists in the client list.
     * @param name The name to check.
     * @return True if the name exists, false otherwise.
     */
    bool nameExists(const std::string& name);

    /**
     * @brief Default destructor.
     */
    ~ClientCmd() = default;
};

#endif //PARSECMD_H