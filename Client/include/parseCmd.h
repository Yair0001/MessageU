#ifndef PARSECMD_H
#define PARSECMD_H

#include <cryptopp/cryptlib.h>
#include <vector>
#include "RSAWrapper.h"
#include "utils.h"
#include <fstream>
#include "ServerHandler.h"

class ClientCmd {
private:

    ServerHandler *serverHandler;

    std::vector<CryptoPP::byte> _cid;
    std::vector<CryptoPP::byte> _version;
    std::vector<CryptoPP::byte> _code;
    std::vector<CryptoPP::byte> _payloadSize;
    std::vector<CryptoPP::byte> _payload;
    std::vector<CryptoPP::byte> _userName;
    std::vector<CryptoPP::byte> _msgType;
    std::vector<CryptoPP::byte> _contentSize;
    std::vector<CryptoPP::byte> _msgContent;
    std::vector<CryptoPP::byte> _otherCid;

    std::vector<CryptoPP::byte> _publicKey;
    std::vector<CryptoPP::byte> _privateKey;

public:
    explicit ClientCmd();
    std::vector<CryptoPP::byte> parseCommand(const std::string& command);
    std::vector<CryptoPP::byte> registerUser();
    ~ClientCmd() = default;

};

#endif //PARSECMD_H
