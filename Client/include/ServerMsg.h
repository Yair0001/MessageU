//
// Created by yairm on 19/01/2025.
//

#ifndef SERVERMSG_H
#define SERVERMSG_H
#include <vector>
#include <cryptopp/trap.h>

class ServerMsg {
private:
    std::vector<CryptoPP::byte> _version;
    std::vector<CryptoPP::byte> _code;
    std::vector<CryptoPP::byte> _payloadSize;
    std::vector<CryptoPP::byte> _payload;

public:
    ServerMsg(std::vector<CryptoPP::byte> msg);
    int getCode() const;
    std::vector<CryptoPP::byte> getPayload() const;
    static bool isValidCode(int code);

};
#endif //SERVERMSG_H
