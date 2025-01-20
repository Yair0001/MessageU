//
// Created by yairm on 19/01/2025.
//
#include "ServerMsg.h"
#include "utils.h"

ServerMsg::ServerMsg(std::vector<CryptoPP::byte> msg) {
    _version.resize(VERSION_SIZE);
    _code.resize(CODE_SIZE);
    _payloadSize.resize(PAYLOAD_SZ_SIZE);

    std::cout << "MSG: ";
    printMsg(msg);

    unsigned int currOffset = 0;

    // Extract version
    for (unsigned int i = 0; i < VERSION_SIZE; i++) {
        _version[i] = msg[currOffset + i];
    }
    std::cout << "VERSION " << bytes_to_hex(_version) << std::endl;
    currOffset += VERSION_SIZE;

    // Extract code
    for (unsigned int i = 0; i < CODE_SIZE; i++) {
        _code[i] = msg[currOffset + i];
    }
    std::cout << "CODE " << bytes_to_hex(_code) << std::endl;
    currOffset += CODE_SIZE;

    // Extract payload size
    for (unsigned int i = 0; i < PAYLOAD_SZ_SIZE; i++) {
        _payloadSize[i] = msg[currOffset + i];
    }
    std::cout << "Payload Size " << bytes_to_hex(_payloadSize) << std::endl;
    currOffset += PAYLOAD_SZ_SIZE;

    // Calculate the actual payload size from _payloadSize (assuming big-endian)
    unsigned int payloadSize = 0;
    for (int i = 0; i < PAYLOAD_SZ_SIZE; i++) {
        payloadSize |= (_payloadSize[i] << (8 * (PAYLOAD_SZ_SIZE - 1 - i)));
    }

    // Resize the payload vector to the size indicated by payloadSize
    _payload.resize(payloadSize);

    // Extract payload
    for (unsigned int i = 0; i < payloadSize; i++) {
        _payload[i] = msg[currOffset + i];
    }
    std::cout << "Payload " << bytes_to_hex(_payload) << std::endl;
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