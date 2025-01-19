#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <fstream>
#include <cryptopp/trap.h>
#include <iostream>

#define CLIENT_VERSION 2
#define INFO_FILE_NAME "my.info"
#define SERVER_INFO_FILE "../server.info"

#define BYTES_TILL_PAYLOAD 7

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
    OK=0xFF,
    REGISTER_ERROR=0x01,
    SERVER_ERROR=9000,
};

enum RequestCodes {
    REGISTER_CODE=600,
    CLIENTS_LIST_CODE=601,
    PUBLIC_KEY_CODE=602,
    SEND_MSG_CODE=603,
    WAITING_LIST_CODE=604,
};

enum AnswerCodes {
    REGISTER_OK=2100,
    CLIENTS_LIST_OK=2101,
    PUBLIC_KEY_OK=2102,
    SEND_MSG_OK=2103,
    WAITING_LIST_OK=2104,
};


std::vector<std::string> splitString(const std::string& s, char del);
std::vector<std::string> readServerInfo(const std::string& path);
std::string bytesToString(const std::vector<CryptoPP::byte> &bytes);
int sumVector(const std::vector<CryptoPP::byte>& bytes);
unsigned char extractByte(int value, int byteIndex);

template <typename T>
void mergeVector(std::vector<T>& res, std::initializer_list<std::vector<T>> vecs) {
    for (const auto& vec : vecs) {
        res.insert(res.end(), vec.begin(), vec.end());
    }
}

#endif //UTILS_H
