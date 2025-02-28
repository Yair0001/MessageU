#include "client.h"

Client::Client(const std::vector<CryptoPP::byte>& cid){
    _cid = cid;
    _publicKey.resize(PUBLIC_KEY_SIZE);
    _symKey.resize(SYM_KEY_SIZE);
}

void Client::setPublicKey(const std::vector<CryptoPP::byte>& publicKey){
    _publicKey = publicKey;
}

void Client::setSymKey(const std::vector<CryptoPP::byte>& symKey){
    _symKey = symKey;
}

std::vector<CryptoPP::byte> Client::getCid() const{
    return _cid;
}
