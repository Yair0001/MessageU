//
// Created by Yair on 15/01/2025.
//
#include "ServerHandler.h"

/**
 * @brief Constructs a ServerHandler object with the specified IP and port.
 * @param ip The server IP address.
 * @param port The server port number.
 */
ServerHandler::ServerHandler(const std::string &ip, int port): _socket(_ioContext) {
    _ip = ip;
    _port = port;
    boost::asio::ip::tcp::resolver resolver(_ioContext);
    const auto endPoints = resolver.resolve(ip, std::to_string(port));
    boost::asio::connect(_socket, endPoints);
}

/**
 * @brief Destructor for the ServerHandler class.
 */
ServerHandler::~ServerHandler() {
    if (_socket.is_open()) {
        _socket.close();
    }
}

/**
 * @brief Sends a message to the server.
 * @param message The message to send.
 */
void ServerHandler::sendMessage(std::vector<CryptoPP::byte> message) {
    boost::asio::write(_socket, boost::asio::buffer(message));
}

/**
 * @brief Receives a message from the server.
 * @return The received message.
 */
std::vector<CryptoPP::byte> ServerHandler::receiveMessage() {
    try {
        // Step 1: Read the initial 7 bytes until the payload
        std::vector<CryptoPP::byte> initialBuffer(BYTES_TILL_PAYLOAD);
        boost::asio::read(_socket, boost::asio::buffer(initialBuffer, BYTES_TILL_PAYLOAD));

        // Step 2: Extract the last 4 bytes as a big-endian integer
        std::uint32_t additionalBytes = 0;
        std::memcpy(&additionalBytes, &initialBuffer[3], sizeof(additionalBytes));
        additionalBytes = ntohl(additionalBytes); // Convert from network to host byte order

        std::vector<CryptoPP::byte> message;
        message.insert(message.end(), initialBuffer.begin(), initialBuffer.end());

        // Step 3: Read the additional bytes
        if (additionalBytes > 0) {
            std::vector<CryptoPP::byte> additionalBuffer(additionalBytes);
            boost::asio::read(_socket, boost::asio::buffer(additionalBuffer, additionalBytes));
            message.insert(message.end(), additionalBuffer.begin(), additionalBuffer.end());
        }

        return message;
    } catch (const std::exception& e) {
        return {};
    }
}

/**
 * @brief Gets the server IP address.
 * @return The server IP address.
 */
const std::string &ServerHandler::getIp() {
    return _ip;
}

/**
 * @brief Gets the server port number.
 * @return The server port number.
 */
int ServerHandler::getPort() {
    return _port;
}