//
// Created by Yair on 15/01/2025.
//

#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <boost/asio.hpp>

class ServerHandler {
private:

public:
    ServerHandler();
    void startConnection();
    void sendToServer();
    ~ServerHandler();
};
#endif //SERVERHANDLER_H
