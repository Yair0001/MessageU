//
// Created by Yair on 15/01/2025.
//

#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <boost/asio.hpp>
#include <cryptopp/cryptlib.h>
#include "utils.h"

/**
 * @class ServerHandler
 * @brief A class for handling server communication.
 *
 * This class provides functionalities to connect to a server, send messages, and receive messages.
 */
class ServerHandler {
private:
    boost::asio::io_context _ioContext; ///< IO context for asynchronous operations.
    boost::asio::ip::tcp::socket _socket; ///< TCP socket for communication.
    std::string _ip; ///< Server IP address.
    int _port; ///< Server port number.

public:
    /**
     * @brief Constructor that initializes the ServerHandler with server IP and port.
     * @param ip The server IP address.
     * @param port The server port number.
     */
    ServerHandler(const std::string& ip, int port);

    /**
     * @brief Sends a message to the server.
     * @param message The message to send.
     */
    void sendMessage(std::vector<CryptoPP::byte> message);

    /**
     * @brief Receives a message from the server.
     * @return The received message.
     */
    std::vector<CryptoPP::byte> receiveMessage();

    /**
     * @brief Gets the server IP address.
     * @return The server IP address.
     */
    const std::string& getIp();

    /**
     * @brief Gets the server port number.
     * @return The server port number.
     */
    int getPort();

    /**
     * @brief Destructor for the ServerHandler class.
     */
    ~ServerHandler();
};

#endif //SERVERHANDLER_H