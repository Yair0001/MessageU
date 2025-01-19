//
// Created by Yair on 15/01/2025.
//

#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <boost/asio.hpp>
#include <cryptopp/cryptlib.h>
#include "utils.h"

class ServerHandler {
private:
    boost::asio::io_context _ioContext;
    boost::asio::ip::tcp::socket _socket;
    std::string _ip;
    int _port;


public:
    ServerHandler(const std::string& ip, int port);
    void sendMessage(std::vector<CryptoPP::byte> message);
    std::vector<CryptoPP::byte> receiveMessage();
    const std::string& getIp();
    int getPort();
    ~ServerHandler();
};
#endif //SERVERHANDLER_H
