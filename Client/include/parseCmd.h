#ifndef PARSECMD_H
#define PARSECMD_H

#include <cryptopp/cryptlib.h>
#include <vector>
#include "utils.h"

class ClientCmd {
private:
    std::vector<CryptoPP::byte> _cid;
    std::vector<CryptoPP::byte> _version;
    std::vector<CryptoPP::byte> _code;
    std::vector<CryptoPP::byte> _payloadSize;
    std::vector<CryptoPP::byte> _payload;
public:
    ClientCmd(const std::string& command);


};

#endif //PARSECMD_H
