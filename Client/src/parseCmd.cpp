#include "parseCmd.h"

#include <iostream>

ClientCmd::ClientCmd() {
        _cid.resize(CLIENT_ID_SIZE);
        _version.resize(VERSION_SIZE);
        _code.resize(CODE_SIZE);
        _payloadSize.resize(PAYLOAD_SZ_SIZE);
        _version[0] = {static_cast<CryptoPP::byte>(CLIENT_VERSION)};
    }

std::vector<CryptoPP::byte> ClientCmd::parseCommand(const std::string& command) {
        int payloadSize = NAME_SIZE+1+PUBLIC_KEY_SIZE;

        switch (command) {
            case "110":
                _userName.resize(NAME_SIZE+1);
                _pubKey.resize(PUBLIC_KEY_SIZE);
                _version[0] = {static_cast<CryptoPP::byte>(CLIENT_VERSION)};
                _code = {(REGISTER_CODE >> 8) & 0xff, REGISTER_CODE & 0xff};
                payloadSize = NAME_SIZE+1 +PUBLIC_KEY_SIZE;
                _payloadSize = {(payloadSize >> 24) & 0xff, (payloadSize >> 16) & 0xff, (payloadSize>> 8) & 0xff, payloadSize & 0xff};
                return registerUser();
            case "120":
                _code = {(CLIENTS_LIST_CODE >> 8) & 0xff, CLIENTS_LIST_CODE & 0xff};
                payloadSize = 0;
                _payloadSize = {(payloadSize >> 24) & 0xff, (payloadSize >> 16) & 0xff, (payloadSize>> 8) & 0xff, payloadSize & 0xff};
                break;
            case "130":
                _code = {(PUBLIC_KEY_CODE >> 8) & 0xff, PUBLIC_KEY_CODE & 0xff};
                payloadSize = CLIENT_ID_SIZE;
                _payloadSize = {(payloadSize >> 24) & 0xff, (payloadSize >> 16) & 0xff, (payloadSize>> 8) & 0xff, payloadSize & 0xff};
                _otherCid.resize(CLIENT_ID_SIZE);
                break;
            case "140":
                _code = {(WAITING_LIST_CODE >> 8) & 0xff, WAITING_LIST_CODE & 0xff};
                payloadSize = 0;
                _payloadSize = {(payloadSize >> 24) & 0xff, (payloadSize >> 16) & 0xff, (payloadSize>> 8) & 0xff, payloadSize & 0xff};
                break;
            case "150":
            case "151":
            case "152":
            case "153":
                _code = {(SEND_MSG_CODE >> 8) & 0xff, SEND_MSG_CODE & 0xff};
                // need to set payload size later because of message type
                _otherCid.resize(CLIENT_ID_SIZE);
                _msgType.resize(MSG_TYPE_SIZE);
                _contentSize.resize(MSG_CONTENT_SIZE);
                break;
            case "0":
                _code = {EXIT,EXIT};
                break;
            default:
                _code = {0x00,0xff};
                break;
        }
    }

std::vector<CryptoPP::byte> ClientCmd::registerUser() {
    std::ifstream userFile("my.info");
    if (userFile.is_open()) {
        return {REGISTER_ERROR};
    }

    std::string userName;

    std::cout << "Enter Username: ";
    std::cin >> userName;
    _userName.resize(NAME_SIZE);
    for (int i = 0; i < NAME_SIZE; i++) {
        if (userName[i] == '\0') {
            _userName[i] = userName[i];
        }
        else {
            userName[i] = '\0';
        }
    }
}