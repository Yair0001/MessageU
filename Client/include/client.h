//
// Created by yairm on 13/01/2025.
//

#ifndef CLIENT_H
#define CLIENT_H
#include <vector>
#include <cryptopp/config_int.h>

class Client
{
private:
    std::vector<CryptoPP::byte> _cid;
    std::vector<CryptoPP::byte> _name;
    std::vector<CryptoPP::byte> _publicKey;
    std::vector<CryptoPP::byte> _symKey;



};
#endif //CLIENT_H
