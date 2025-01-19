#include "parseCmd.h"

ClientCmd::ClientCmd(ServerHandler& serverHandler)
    : _serverHandler(serverHandler.getIp(),serverHandler.getPort())
{
    _cid.resize(CLIENT_ID_SIZE);
    _version.resize(VERSION_SIZE);
    _code.resize(CODE_SIZE);
    _payloadSize.resize(PAYLOAD_SZ_SIZE);
    _version = {static_cast<CryptoPP::byte>(CLIENT_VERSION)};
}

std::vector<CryptoPP::byte> ClientCmd::parseCommand(const std::string& command) {
        int payloadSize = NAME_SIZE+1+PUBLIC_KEY_SIZE;

        switch (std::stoi(command)) {
            case 110:
                _userName.resize(NAME_SIZE+1);
                _publicKey.resize(PUBLIC_KEY_SIZE);
                _version[0] = {static_cast<CryptoPP::byte>(CLIENT_VERSION)};
                _code = {(REGISTER_CODE >> 8) & 0xff, REGISTER_CODE & 0xff};
                payloadSize = NAME_SIZE+1 +PUBLIC_KEY_SIZE;
                _payloadSize = {
                extractByte(payloadSize, 3),
                extractByte(payloadSize, 2),
                extractByte(payloadSize, 1),
                extractByte(payloadSize, 0)
                };
                return registerUser();
            case 120:
                _code = {(CLIENTS_LIST_CODE >> 8) & 0xff, CLIENTS_LIST_CODE & 0xff};
                payloadSize = 0;
                _payloadSize = {
                extractByte(payloadSize, 3),
                extractByte(payloadSize, 2),
                extractByte(payloadSize, 1),
                extractByte(payloadSize, 0)
                };
                break;
            case 130:
                _code = {(PUBLIC_KEY_CODE >> 8) & 0xff, PUBLIC_KEY_CODE & 0xff};
                payloadSize = CLIENT_ID_SIZE;
                _payloadSize = {
                extractByte(payloadSize, 3),
                extractByte(payloadSize, 2),
                extractByte(payloadSize, 1),
                extractByte(payloadSize, 0)
                };
                _otherCid.resize(CLIENT_ID_SIZE);
                break;
            case 140:
                _code = {(WAITING_LIST_CODE >> 8) & 0xff, WAITING_LIST_CODE & 0xff};
                payloadSize = 0;
                _payloadSize = {
                extractByte(payloadSize, 3),
                extractByte(payloadSize, 2),
                extractByte(payloadSize, 1),
                extractByte(payloadSize, 0)
                };
                break;
            case 150:
            case 151:
            case 152:
            case 153:
                _code = {(SEND_MSG_CODE >> 8) & 0xff, SEND_MSG_CODE & 0xff};
                // need to set payload size later because of message type
                _otherCid.resize(CLIENT_ID_SIZE);
                _msgType.resize(MSG_TYPE_SIZE);
                _contentSize.resize(MSG_CONTENT_SIZE);
                break;
            case 0:
                _code = {EXIT,EXIT};
                break;
            default:
                _code = {0x00,0xff};
                break;
        }
    }

std::vector<CryptoPP::byte> ClientCmd::registerUser() {
    std::ifstream userFile(INFO_FILE_NAME);
    if (userFile.is_open()) {
        return {REGISTER_ERROR};
    }

    // get userName
    std::string userName;

    std::cout << "Enter Username: ";
    std::getline(std::cin, userName);

    if (userName.length() > NAME_SIZE) {
        userName = userName.substr(0, NAME_SIZE) + '\0';// Truncate input
    }

    _userName.resize(NAME_SIZE);
    for (int i = 0; i < NAME_SIZE; i++) {
        if (userName[i] != '\0') {
            _userName[i] = userName[i];
        }
        else {
            _userName[i] = '\0';
        }
    }

    // generate pub and private keys
    RSAPrivateWrapper rsa_private_wrapper;
    std::string pubKeyStr = rsa_private_wrapper.getPublicKey();

    for (int i = 0; i < PUBLIC_KEY_SIZE; i++) {
        _publicKey[i] = pubKeyStr[i];
    }

    std::vector<CryptoPP::byte> msgToSend{};
    mergeVector<CryptoPP::byte>(msgToSend, {_cid, _version, _code, _payloadSize, _userName, _publicKey});
    _serverHandler.sendMessage(msgToSend);
    ServerMsg msgToReceive = ServerMsg(_serverHandler.receiveMessage());
    if (msgToReceive.getCode() == SERVER_ERROR) {
        return {SERVER_ERROR >> 8 & 0xff, SERVER_ERROR & 0xff};
    }

    std::ofstream infoFile(INFO_FILE_NAME);
    if (infoFile.is_open()) {
        infoFile << bytesToString(_userName) << '\n';
        infoFile << bytesToString(msgToReceive.getPayload()) << '\n';
        infoFile << Base64Wrapper::encode(rsa_private_wrapper.getPublicKey());
        return {OK};
    }
    else {
        std::cout << "Error while writing to file" << std::endl;
        return {SERVER_ERROR >> 8 & 0xff, SERVER_ERROR & 0xff};
    }
}