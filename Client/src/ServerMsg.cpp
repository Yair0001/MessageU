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
    std::cout << "VERSION " << bytesToHex(_version) << std::endl;
    currOffset += VERSION_SIZE;

    // Extract code
    for (unsigned int i = 0; i < CODE_SIZE; i++) {
        _code[i] = msg[currOffset + i];
    }
    std::cout << "CODE " << bytesToHex(_code) << std::endl;
    currOffset += CODE_SIZE;

    // Extract payload size
    for (unsigned int i = 0; i < PAYLOAD_SZ_SIZE; i++) {
        _payloadSize[i] = msg[currOffset + i];
    }
    std::cout << "Payload Size " << bytesToHex(_payloadSize) << std::endl;
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
    std::cout << "Payload " << bytesToHex(_payload) << std::endl;
}

std::vector<CryptoPP::byte> ServerMsg::getCode() const {
    return _code;
}

std::vector<CryptoPP::byte> ServerMsg::getPayload() const {
    return _payload;
}

bool ServerMsg::isValidCode(const int code) {
    return code == AnswerCodes::REGISTER_OK || code == AnswerCodes::CLIENTS_LIST_OK || code == AnswerCodes::PUBLIC_KEY_OK || code == AnswerCodes::SEND_MSG_OK || code == AnswerCodes::WAITING_LIST_OK;
}
bool ServerMsg::errorsExist(const ServerMsg& ans) {
    const int code = bytesToType<int>(ans.getCode());
    if (isValidCode(code) == false) {
        return true;
    }
    return false;
}

void ServerMsg::printError(const ServerMsg& ans) {
    const int code = bytesToType<int>(ans.getCode());
    if (code == ErrorCodes::ALREADY_REGISTERED) {
        std::cerr << "ALREADY_REGISTERED" << std::endl;
    }
    else if (code == ErrorCodes::NOT_REGISTERED) {
        std::cerr << "NOT_REGISTERED" << std::endl;
    }
    else if (code == ErrorCodes::NO_CLIENTS) {
        std::cerr << "NO_CLIENTS" << std::endl;
    }
    else if (code == ErrorCodes::SERVER_ERROR) {
        std::cerr << "SERVER_ERROR" << std::endl;
    }

}

std::vector<CryptoPP::byte> ServerMsg::getPayloadSizeVec() const {
    return _payloadSize;
}

int ServerMsg::getPayloadSizeInt() const {
    return bytesToType<int>(_payloadSize);
}

void ServerMsg::printClientsList(const std::vector<std::vector<CryptoPP::byte>>& clients) {
    std::vector<CryptoPP::byte> currName{};
    std::vector<CryptoPP::byte> currCID(CLIENT_ID_SIZE);

    for (unsigned int i = 0; i < clients.size(); i++) {
        for (unsigned int j = 0; j < clients[i].size(); j++) {
            if (j < CLIENT_ID_SIZE) {
                currCID[j] = clients[i][j];
            }
            else if (clients[i][j] != 0x00) {
                currName.push_back(clients[i][j]);
            }
        }

        std::cout << "CLIENT " << i << " Name: " << bytesToString(currName) << std::endl;
        std::cout << "CLIENT " << i << " CID: " << bytesToHex(currCID) << std::endl;
        currName.clear();
    }
}