//
// Created by yairm on 19/01/2025.
//
#include "ServerMsg.h"

#include <unordered_map>

#include "client.h"
#include "utils.h"

ServerMsg::ServerMsg(const std::vector<CryptoPP::byte>& msg) {
    _version.resize(VERSION_SIZE);
    _code.resize(CODE_SIZE);
    _payloadSize.resize(PAYLOAD_SZ_SIZE);

    // std::cout << "MSG: ";
    // printMsg(msg);

    unsigned int currOffset = 0;

    // Extract version
    for (unsigned int i = 0; i < VERSION_SIZE; i++) {
        _version[i] = msg[currOffset + i];
    }
    // std::cout << "VERSION " << bytesToHex(_version) << std::endl;
    currOffset += VERSION_SIZE;

    // Extract code
    for (unsigned int i = 0; i < CODE_SIZE; i++) {
        _code[i] = msg[currOffset + i];
    }
    // std::cout << "CODE " << bytesToHex(_code) << std::endl;
    currOffset += CODE_SIZE;

    // Extract payload size
    for (unsigned int i = 0; i < PAYLOAD_SZ_SIZE; i++) {
        _payloadSize[i] = msg[currOffset + i];
    }
    // std::cout << "Payload Size " << bytesToHex(_payloadSize) << std::endl;
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
    // std::cout << "Payload " << bytesToHex(_payload) << std::endl;
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
    else if (code == ErrorCodes::NO_CLIENTS_ERROR) {
        std::cerr << "NO_CLIENTS" << std::endl;
    }
    else if (code == ErrorCodes::SERVER_ERROR) {
        std::cerr << "SERVER_ERROR" << std::endl;
    }
    else if (code == ErrorCodes::NO_CODE_ERROR){
        std::cerr << "NO_CODE_ERROR" << std::endl;
    }
    else if (code == ErrorCodes::NO_CLIENT_NAME_ERROR)
    {
        std::cerr << "NO_CLIENT_NAME_ERROR" << std::endl;
    }

}

std::vector<CryptoPP::byte> ServerMsg::getPayloadSizeVec() const {
    return _payloadSize;
}

int ServerMsg::getPayloadSizeInt() const {
    return bytesToType<int>(_payloadSize);
}

void ServerMsg::printClientsList(const std::vector<std::vector<CryptoPP::byte>>& clients, boost::bimap<std::string, Client>& clientList){
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

        clientList.insert({bytesToString(currName),Client(currCID)});

        std::cout << "CLIENT " << i << " Name: " << bytesToString(currName) << std::endl;
        std::cout << "CLIENT " << i << " CID: " << bytesToHex(currCID) << std::endl;
        currName.clear();
    }
}

void ServerMsg::printMsg(const std::vector<CryptoPP::byte>& msg, RSAPrivateWrapper& privateKey, boost::bimap<std::string, Client>& clientList){
    std::string usrName;
    std::vector<CryptoPP::byte> usrCid{};
    std::vector<CryptoPP::byte> msgId{};
    std::vector<CryptoPP::byte> msgTypeBytes{};
    std::vector<CryptoPP::byte> msgSize{};
    std::vector<CryptoPP::byte> msgContent{};

    for (unsigned int i = 0; i < msg.size(); i++){
        if (i < CLIENT_ID_SIZE) usrCid.push_back(msg[i]);
        else if (i < CLIENT_ID_SIZE + MSG_ID_SIZE) msgId.push_back(msg[i]);
        else if (i < CLIENT_ID_SIZE + MSG_ID_SIZE + MSG_TYPE_SIZE) msgTypeBytes.push_back(msg[i]);
        else if (i < CLIENT_ID_SIZE + MSG_ID_SIZE + MSG_TYPE_SIZE + MSG_SZ_SIZE) msgSize.push_back(msg[i]);
        else msgContent.push_back(msg[i]);
    }

    for (const auto& pair : clientList.right){
        if (pair.first.getCid() == usrCid){
            usrName = pair.second;
        }
    }

    const int msgTypeInt = bytesToType<int>(msgTypeBytes);
    if (msgTypeInt == SYM_KEY_REQ){
        if (clientList.left.find(usrName)->second.pubKeyExists()){
            std::cout << "From: " << usrName << std::endl;
            std::cout << "Content:\n" << "Request for symmetric key\n";
            std::cout << "-----<EOM>-----\n";
        }
        else{
            std::cout << "Public key does not exist\n";
        }

    }
    else if (msgTypeInt == SYM_KEY_SEND){
        const auto it = clientList.left.find(usrName);
        const std::string msgContentDecrypted = privateKey.decrypt(bytesToString(msgContent));
        if (it != clientList.left.end()) {
            Client clientCopy = it->second;

            clientCopy.setSymKey(stringToBytes(msgContentDecrypted));

            clientList.left.erase(it);
            clientList.insert({usrName, clientCopy});
        }

        std::cout << "From: " << usrName << std::endl;
        std::cout << "Content:\n" << "symmetric key received\n";
        std::cout << msgContentDecrypted << std::endl;
        std::cout << "-----<EOM>-----\n";
    }
    else if (msgTypeInt == TEXT_SEND){
        if (clientList.left.find(usrName) != clientList.left.end()){
            if (clientList.left.find(usrName)->second.symKeyExists()){
                AESWrapper symKey(reinterpret_cast<const unsigned char*>(bytesToString(clientList.left.find(usrName)->second.getSymKey()).c_str()),SYM_KEY_SIZE);

                std::string msgContentDecrypted = bytesToString(msgContent);
                msgContentDecrypted = symKey.decrypt(msgContentDecrypted.c_str(),msgContentDecrypted.length());

                std::cout << "From: " << usrName << std::endl;
                std::cout << "Content:\n" << msgContentDecrypted << std::endl;
                std::cout << "-----<EOM>-----\n";

            }
            else{
                std::cout << "can't decrypt message" << std::endl;
            }
        }
    }
}