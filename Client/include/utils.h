#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <fstream>

#define CLIENT_VERSION 2
#define SERVER_INFO_FILE "../server.info"

enum ProtocolSizes{
    CLIENT_ID_SIZE = 16,
    VERSION_SIZE=1,
    CODE_SIZE=2,
    PAYLOAD_SZ_SIZE=4,
    NAME_SIZE=254, // + 1 \x00 byte
    PUBLIC_KEY_SIZE=160,
    MSG_TYPE_SIZE=1,
    MSG_CONTENT_SIZE=4,
};


std::vector<std::string> splitString(const std::string& s, char del);
std::vector<std::string> readServerInfo(const std::string& path);

#endif //UTILS_H
