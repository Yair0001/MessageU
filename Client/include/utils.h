#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <fstream>

#define CLIENT_VERSION 2
#define SERVER_INFO_FILE "../server.info"

enum ProtocolSizes{
    CLIENT_ID_SIZE=16,
    VERSION_SIZE=1,
    CODE_SIZE=2,
    PAYLOAD_SZ_SIZE=4,
    NAME_SIZE=254, // + 1 \x00 byte
    PUBLIC_KEY_SIZE=160,
    MSG_TYPE_SIZE=1,
    MSG_CONTENT_SIZE=4,
};

enum ErrorCodes{
    EXIT=0x00,
    REGISTER_ERROR=0x01,
};

enum RequestCodes {
    REGISTER_CODE=600,
    CLIENTS_LIST_CODE=601,
    PUBLIC_KEY_CODE=602,
    SEND_MSG_CODE=603,
    WAITING_LIST_CODE=604,
};


std::vector<std::string> splitString(const std::string& s, char del);
std::vector<std::string> readServerInfo(const std::string& path);

#endif //UTILS_H
