//
// Created by yairm on 19/01/2025.
//
#include "ServerMsg.h"
#include "utils.h"

ServerMsg::ServerMsg(std::vector<CryptoPP::byte> msg) {
    for (unsigned int i = 0; i < VERSION_SIZE; i++) {
        _version[i] = msg[i];
    }
    for (unsigned int i = VERSION_SIZE; i < CODE_SIZE; i++) {
        _code[i] = msg[i];
    }
    for (unsigned int i = CODE_SIZE; i < PAYLOAD_SZ_SIZE; i++) {
        _payloadSize[i] = msg[i];
    }
    for (unsigned int i = PAYLOAD_SZ_SIZE; i < sumVector(_payloadSize); i++) {
        _payload[i] = msg[i];
    }
}
int ServerMsg::getCode() const {
    return sumVector(_code);
}

std::vector<CryptoPP::byte> ServerMsg::getPayload() const {
    return _payload;
}

bool ServerMsg::isValidCode(const int code) {
    return code == AnswerCodes::REGISTER_OK || code == AnswerCodes::CLIENTS_LIST_OK || code == AnswerCodes::PUBLIC_KEY_OK || code == AnswerCodes::SEND_MSG_OK || code == AnswerCodes::WAITING_LIST_OK;
}