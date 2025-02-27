#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iomanip>
#include <vector>
#include <fstream>
#include <cryptopp/trap.h>
#include <iostream>
#include <cstdint>
#include <sstream>

#define CLIENT_VERSION 2
#define INFO_FILE_NAME "my.info"
#define SERVER_INFO_FILE "../server.info"

#define BYTES_TILL_PAYLOAD (VERSION_SIZE+CODE_SIZE+PAYLOAD_SZ_SIZE)

enum ProtocolSizes{
    CLIENT_ID_SIZE=16,
    VERSION_SIZE=1,
    CODE_SIZE=2,
    PAYLOAD_SZ_SIZE=4,
    NAME_SIZE=255,
    PUBLIC_KEY_SIZE=160,
    SYM_KEY_SIZE=16,
    MSG_TYPE_SIZE=1,
    MSG_CONTENT_SIZE=4,
};

enum ErrorCodes{
    EXIT=0x00,
    OK=0xFF,
    ALREADY_REGISTERED=0x01,
    NOT_REGISTERED=0x02,
    NO_CLIENTS=0x03,
    INVALID_CID=0x04,
    SERVER_ERROR=9000,
    NO_CODE_ERROR=9001,
    NO_CLIENT_NAME_ERROR=9002,
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
void printMsg(std::vector<CryptoPP::byte> msg);
void printMsgString(std::vector<CryptoPP::byte> msg);
std::string bytesToHex(const std::vector<CryptoPP::byte>& bytes);
std::vector<CryptoPP::byte> numOfBytes(std::vector<CryptoPP::byte> bytes, int start, int end);
std::vector<CryptoPP::byte> stringToBytes(const std::string& str);
std::vector<std::string> getFileContents(std::ifstream& file);
std::vector<CryptoPP::byte> hexStrToBytes(const std::string& hexString);
std::vector<CryptoPP::byte> getCidOfInfoFile(std::ifstream& file);


template <typename T>
void mergeVector(std::vector<T>& res, std::initializer_list<std::vector<T>> vecs) {
    for (const auto& vec : vecs) {
        res.insert(res.end(), vec.begin(), vec.end());
    }
}

template <typename T>
std::vector<CryptoPP::byte> getBytesAsCryptoPP(T value, const size_t numOfBytes) {
    static_assert(std::is_integral<T>::value || std::is_enum<T>::value,
                  "T must be an integral or enum type");

    if (numOfBytes > sizeof(T)) {
        throw std::invalid_argument("numOfBytes exceeds the size of the input value type");
    }

    std::vector<CryptoPP::byte> bytes(numOfBytes);
    for (size_t i = 0; i < numOfBytes; ++i) {
        bytes[numOfBytes - 1 - i] = static_cast<CryptoPP::byte>((value >> (i * 8)) & 0xFF);
    }
    return bytes;
}

template <typename T>
T bytesToType(const std::vector<CryptoPP::byte>& bytes) {
    static_assert(std::is_integral<T>::value, "T must be an integral type");

    if (bytes.size() > sizeof(T)) {
        throw std::invalid_argument("Byte vector size exceeds the size of the output type");
    }

    T value = 0;
    for (unsigned char byte : bytes) {
        value = (value << 8) | static_cast<T>(byte);
    }
    return value;
}

#endif //UTILS_H
