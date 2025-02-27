//
// Created by yairm on 19/01/2025.
//

#ifndef SERVERMSG_H
#define SERVERMSG_H
#include <vector>
#include <cryptopp/trap.h>
#include "client.h"
#include <unordered_map>

class ServerMsg {
private:
    std::vector<CryptoPP::byte> _version;
    std::vector<CryptoPP::byte> _code;
    std::vector<CryptoPP::byte> _payloadSize;
    std::vector<CryptoPP::byte> _payload;

public:
    explicit ServerMsg(const std::vector<CryptoPP::byte>& msg);
    std::vector<CryptoPP::byte> getCode() const;
    std::vector<CryptoPP::byte> getPayloadSizeVec() const;
    int getPayloadSizeInt() const;
    std::vector<CryptoPP::byte> getPayload() const;
    static bool isValidCode(int code);
    static bool errorsExist(const ServerMsg& ans);
    static void printError(const ServerMsg& ans);
    static void printClientsList(const std::vector<std::vector<CryptoPP::byte>>& clients, std::unordered_map<std::string, Client>& clientList);


};
#endif //SERVERMSG_H
