//
// Created by yairm on 13/01/2025.
//

#ifndef CLIENT_H
#define CLIENT_H
#include <vector>
#include <cryptopp/config_int.h>
#include "utils.h"

class Client
{
private:
    std::vector<CryptoPP::byte> _cid;
    std::vector<CryptoPP::byte> _publicKey;
    std::vector<CryptoPP::byte> _symKey;

    bool _hasPublicKey = false;
    bool _hasSymKey = false;

public:
    Client() = default;
    explicit Client(const std::vector<CryptoPP::byte>& cid);
    void setPublicKey(const std::vector<CryptoPP::byte>& publicKey);
    void setSymKey(const std::vector<CryptoPP::byte>& symKey);
    std::vector<CryptoPP::byte> getCid() const;
    std::vector<CryptoPP::byte> getPublicKey() const;
    std::vector<CryptoPP::byte> getSymKey() const;
    bool pubKeyExists() const;
    bool symKeyExists() const;
};
#endif //CLIENT_H
