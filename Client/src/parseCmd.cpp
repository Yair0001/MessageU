#include "parseCmd.h"

/**
 * @brief Constructs a ClientCmd object with a specified ServerHandler.
 * @param serverHandler Reference to a ServerHandler object.
 */
ClientCmd::ClientCmd(ServerHandler& serverHandler)
    : _serverHandler(serverHandler.getIp(), serverHandler.getPort())
{
    _cid.resize(CLIENT_ID_SIZE);
    _version.resize(VERSION_SIZE);
    _code.resize(CODE_SIZE);
    _payloadSize.resize(PAYLOAD_SZ_SIZE);
    _msgType.resize(MSG_TYPE_SIZE);
    _version = getBytesAsCryptoPP(CLIENT_VERSION, VERSION_SIZE);
}

/**
 * @brief Parses a command string and returns the corresponding byte vector.
 * @param command The command string to parse.
 * @param gottenClientsList has the client list been initialized
 * @return A vector of bytes representing the parsed command.
 */
std::vector<CryptoPP::byte> ClientCmd::parseCommand(const std::string& command, bool gottenClientsList)
{
    _gotClientsList = gottenClientsList;

    std::vector<CryptoPP::byte> res;
    switch (safe_stoi(command))
    {
    case 110:
        _userName.resize(NAME_SIZE);
        _publicKey.resize(PUBLIC_KEY_SIZE);
        _code = getBytesAsCryptoPP(REGISTER_CODE, CODE_SIZE);
        _payloadSize = getBytesAsCryptoPP(NAME_SIZE + PUBLIC_KEY_SIZE, PAYLOAD_SZ_SIZE);
        return registerUser();
    case 120:
        _code = getBytesAsCryptoPP(CLIENTS_LIST_CODE, CODE_SIZE);
        _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
        return clientsList();
    case 130:
        if (!_gotClientsList)
        {
            _code = getBytesAsCryptoPP(NO_CLIENTS_LIST_ERROR, CODE_SIZE);
            _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
            mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
            return res;
        }
        _code = getBytesAsCryptoPP(PUBLIC_KEY_CODE, CODE_SIZE);
        _payloadSize = getBytesAsCryptoPP(CLIENT_ID_SIZE, PAYLOAD_SZ_SIZE);
        _otherCid.resize(CLIENT_ID_SIZE);
        return getPublicKeyOfCid();
    case 140:
        if (!_gotClientsList)
        {
            _code = getBytesAsCryptoPP(NO_CLIENTS_LIST_ERROR, CODE_SIZE);
            _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
            mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
            return res;
        }
        _code = getBytesAsCryptoPP(WAITING_LIST_CODE, CODE_SIZE);
        _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
        return waitingMsgs();
    case 150:
        if (!_gotClientsList)
        {
            _code = getBytesAsCryptoPP(NO_CLIENTS_LIST_ERROR, CODE_SIZE);
            _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
            mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
            return res;
        }
        _code = getBytesAsCryptoPP(SEND_MSG_CODE, CODE_SIZE);
        _msgType = getBytesAsCryptoPP(TEXT_SEND, MSG_TYPE_SIZE);
        _otherCid.resize(CLIENT_ID_SIZE);
        _contentSize.resize(MSG_SZ_SIZE);
        return sendMsg();
    case 151:
        if (!_gotClientsList)
        {
            _code = getBytesAsCryptoPP(NO_CLIENTS_LIST_ERROR, CODE_SIZE);
            _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
            mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
            return res;
        }
        _code = getBytesAsCryptoPP(SEND_MSG_CODE, CODE_SIZE);
        _msgType = getBytesAsCryptoPP(SYM_KEY_REQ, MSG_TYPE_SIZE);
        _otherCid.resize(CLIENT_ID_SIZE);
        _contentSize.resize(MSG_SZ_SIZE);
        return sendMsg();
    case 152:
        if (!_gotClientsList)
        {
            _code = getBytesAsCryptoPP(NO_CLIENTS_LIST_ERROR, CODE_SIZE);
            _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
            mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
            return res;
        }
        _code = getBytesAsCryptoPP(SEND_MSG_CODE, CODE_SIZE);
        _msgType = getBytesAsCryptoPP(SYM_KEY_SEND, MSG_TYPE_SIZE);
        _otherCid.resize(CLIENT_ID_SIZE);
        _contentSize.resize(MSG_SZ_SIZE);
        return sendMsg();
    case 153:
        if (!_gotClientsList)
        {
            _code = getBytesAsCryptoPP(NO_CLIENTS_LIST_ERROR, CODE_SIZE);
            _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
            mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
            return res;
        }
        _msgType = getBytesAsCryptoPP(FILE_SEND, MSG_TYPE_SIZE);
        _code = getBytesAsCryptoPP(SEND_MSG_CODE, CODE_SIZE);
    // need to set payload size later because of message type
        _otherCid.resize(CLIENT_ID_SIZE);
        _contentSize.resize(MSG_SZ_SIZE);
        return sendMsg();
    case 0:
        _code = getBytesAsCryptoPP(EXIT, CODE_SIZE);
        _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
        mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
    default:
        _code = getBytesAsCryptoPP(NO_CODE_ERROR, CODE_SIZE);
        _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
        mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
        return res;
    }
}

/**
 * @brief Registers a user and returns the corresponding byte vector.
 * @return A vector of bytes representing the registration command.
 */
std::vector<CryptoPP::byte> ClientCmd::registerUser()
{
    std::vector<CryptoPP::byte> res;

    std::ifstream userFile(INFO_FILE_NAME);
    if (userFile.is_open())
    {
        _code = getBytesAsCryptoPP(ALREADY_REGISTERED, CODE_SIZE);
        _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
        mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
        return res;
    }
    // get userName
    std::string userName;

    std::cout << "Enter Username: ";
    std::getline(std::cin, userName);

    if (!userName.empty() && userName[userName.length() - 1] == '\n')
    {
        userName.pop_back();
    }

    if (userName.length() > NAME_SIZE - 1)
    {
        userName = userName.substr(0, NAME_SIZE - 1) + '\x00'; // Truncate input
    }

    for (int i = 0; i < NAME_SIZE; i++)
    {
        if (userName[i] != '\0' && i < userName.length())
        {
            _userName[i] = userName[i];
        }
        else
        {
            _userName[i] = '\0';
        }
    }
    std::cout << _userName.size() << '\n';

    // generate pub and private keys
    RSAPrivateWrapper rsa_private_wrapper;
    std::string pubKeyStr = rsa_private_wrapper.getPublicKey();

    for (int i = 0; i < PUBLIC_KEY_SIZE; i++)
    {
        _publicKey[i] = pubKeyStr[i];
    }

    std::vector<CryptoPP::byte> msgToSend{};
    mergeVector<CryptoPP::byte>(msgToSend, {_cid, _version, _code, _payloadSize, _userName, _publicKey});
    _serverHandler.sendMessage(msgToSend);
    ServerMsg msgToReceive = ServerMsg(_serverHandler.receiveMessage());
    if (bytesToType<int>(msgToReceive.getCode()) == SERVER_ERROR)
    {
        _code = getBytesAsCryptoPP(SERVER_ERROR, CODE_SIZE);
        _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
        mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
        return res;
    }

    FILE* infoFile = fopen(INFO_FILE_NAME, "a");
    if (infoFile != nullptr)
    {
        std::vector<CryptoPP::byte> curCid = numOfBytes(msgToReceive.getPayload(), 0, CLIENT_ID_SIZE);

        auto privKey = Base64Wrapper::encode(rsa_private_wrapper.getPrivateKey());
        privKey.erase(std::remove(privKey.begin(), privKey.end(), '\n'), privKey.end());

        fprintf(infoFile, "%s\n", userName.c_str());
        fprintf(infoFile, "%s\n", bytesToHex(curCid).c_str());
        fprintf(infoFile, "%s\n", privKey.c_str());
        fclose(infoFile);

        _code = getBytesAsCryptoPP(REGISTER_OK, CODE_SIZE);
        _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
        mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
        return res;
    }
    std::cerr << "Failed to open the file!" << std::endl;

    _code = getBytesAsCryptoPP(SERVER_ERROR, CODE_SIZE);
    _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
    mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
    return res;
}

/**
 * @brief Retrieves the list of clients and returns the corresponding byte vector.
 * @return A vector of bytes representing the clients list command.
 */
std::vector<CryptoPP::byte> ClientCmd::clientsList()
{
    std::vector<CryptoPP::byte> res;

    std::ifstream userFile(INFO_FILE_NAME);
    if (!userFile.is_open())
    {
        std::cout << "FILE DIDNT OPEN\n";
        _code = getBytesAsCryptoPP(NOT_REGISTERED, CODE_SIZE);
        _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
        mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
        return res;
    }

    _cid = getCidOfInfoFile(userFile);

    std::vector<CryptoPP::byte> msgToSend{};
    mergeVector<CryptoPP::byte>(msgToSend, {_cid, _version, _code, _payloadSize});
    _serverHandler.sendMessage(msgToSend);
    ServerMsg msgToReceive(_serverHandler.receiveMessage());

    if (bytesToType<int>(msgToReceive.getCode()) == SERVER_ERROR)
    {
        _code = getBytesAsCryptoPP(NO_CLIENTS_ERROR, CODE_SIZE);
        _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
        mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
        return res;
    }

    std::vector<CryptoPP::byte> ansPayload = msgToReceive.getPayload();
    int currOffset = 0;
    int ansPayloadSize = msgToReceive.getPayloadSizeInt();
    int numOfClients = ansPayloadSize / (CLIENT_ID_SIZE + NAME_SIZE);
    std::vector<std::vector<CryptoPP::byte>> clients(numOfClients);
    for (int i = 0; i < numOfClients; i++)
    {
        std::vector<CryptoPP::byte> currentClient(CLIENT_ID_SIZE + NAME_SIZE);
        for (int j = 0; j < CLIENT_ID_SIZE + NAME_SIZE; j++)
        {
            currentClient[j] = ansPayload[currOffset + j];
        }
        clients[i] = currentClient;
        currOffset += CLIENT_ID_SIZE + NAME_SIZE;
    }

    ServerMsg::printClientsList(clients, _clientList);
    _code = getBytesAsCryptoPP(CLIENTS_LIST_OK, CODE_SIZE);
    _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
    mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
    return res;
}

/**
 * @brief Retrieves the public key of a client ID and returns the corresponding byte vector.
 * @return A vector of bytes representing the get public key command.
 */
std::vector<CryptoPP::byte> ClientCmd::getPublicKeyOfCid()
{
    std::vector<CryptoPP::byte> res;

    std::ifstream userFile(INFO_FILE_NAME);
    if (!userFile.is_open())
    {
        std::cout << "FILE DIDNT OPEN\n";
        _code = getBytesAsCryptoPP(NOT_REGISTERED, CODE_SIZE);
        _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
        mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
        return res;
    }

    std::string otherName;

    std::cout << "Enter User Name To get public key of: ";
    std::getline(std::cin, otherName);

    if (!otherName.empty() && otherName[otherName.length() - 1] == '\n')
    {
        otherName.pop_back();
    }

    if (otherName.length() > NAME_SIZE)
    {
        otherName = otherName.substr(0, NAME_SIZE); // Truncate input
    }

    if (nameExists(otherName))
    {
        _otherCid = _clientList.left.find(otherName)->second.getCid();
    }
    else
    {
        _code = getBytesAsCryptoPP(NO_CLIENT_NAME_ERROR, CODE_SIZE);
        _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
        mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
        return res;
    }

    std::vector<CryptoPP::byte> msgToSend{};
    mergeVector<CryptoPP::byte>(msgToSend, {_cid, _version, _code, _payloadSize, _otherCid});
    _serverHandler.sendMessage(msgToSend);
    auto msgToReceive = ServerMsg(_serverHandler.receiveMessage());
    if (bytesToType<int>(msgToReceive.getCode()) == SERVER_ERROR)
    {
        _code = getBytesAsCryptoPP(SERVER_ERROR, CODE_SIZE);
        _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
        mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
        return res;
    }

    _code = getBytesAsCryptoPP(PUBLIC_KEY_OK, CODE_SIZE);
    _payloadSize = msgToReceive.getPayloadSizeVec();
    _payload = msgToReceive.getPayload();
    mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize, _payload});

    std::vector<CryptoPP::byte> currentPublicKey{};
    for (size_t i = 0; i < PUBLIC_KEY_SIZE; i++)
    {
        currentPublicKey.push_back(_payload[i + CLIENT_ID_SIZE]);
    }

    auto it = _clientList.left.find(otherName);
    if (it != _clientList.left.end())
    {
        Client updatedClient = it->second; // Copy the existing client
        updatedClient.setPublicKey(currentPublicKey); // Modify it
        _clientList.left.replace_data(it, updatedClient); // Replace in the bimap
    }

    return res;
}

/**
 * @brief Retrieves the waiting messages and returns the corresponding byte vector.
 * @return A vector of bytes representing the waiting messages command.
 */
std::vector<CryptoPP::byte> ClientCmd::waitingMsgs()
{
    std::vector<CryptoPP::byte> res;

    std::ifstream userFile(INFO_FILE_NAME);
    if (!userFile.is_open())
    {
        std::cout << "FILE DIDNT OPEN\n";
        _code = getBytesAsCryptoPP(NOT_REGISTERED, CODE_SIZE);
        _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
        mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
        return res;
    }

    auto fileContents = getFileContents(userFile);
    _cid = hexStrToBytes(fileContents[1]);
    _privateKey = stringToBytes(fileContents[2]);

    std::vector<CryptoPP::byte> msgToSend{};
    mergeVector<CryptoPP::byte>(msgToSend, {_cid, _version, _code, _payloadSize});
    _serverHandler.sendMessage(msgToSend);
    ServerMsg msgToReceive(_serverHandler.receiveMessage());

    if (bytesToType<int>(msgToReceive.getCode()) == SERVER_ERROR)
    {
        _code = getBytesAsCryptoPP(NO_MESSAGES_ERROR, CODE_SIZE);
        _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
        mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
        return res;
    }


    std::vector<CryptoPP::byte> ansPayload = msgToReceive.getPayload();
    int ansPayloadSize = msgToReceive.getPayloadSizeInt();
    std::vector<CryptoPP::byte> currMsgContentSize;
    std::vector<CryptoPP::byte> currMessage{};
    size_t i = VERSION_SIZE+CODE_SIZE+PAYLOAD_SZ_SIZE;
    while (i < ansPayloadSize)
    {
        size_t j;
        for (j = 0; j < CLIENT_ID_SIZE + MSG_ID_SIZE + MSG_TYPE_SIZE; j++)
        {
            currMessage.push_back(ansPayload[i + j]);
        }
        for (; j < CLIENT_ID_SIZE + MSG_ID_SIZE + MSG_TYPE_SIZE + MSG_SZ_SIZE; j++)
        {
            currMessage.push_back(ansPayload[i + j]);
            currMsgContentSize.push_back(ansPayload[i + j]);
        }
        for (; j < CLIENT_ID_SIZE + MSG_ID_SIZE + MSG_TYPE_SIZE + MSG_SZ_SIZE + bytesToType<int>(currMsgContentSize); j++)
        {
            currMessage.push_back(ansPayload[i + j]);
        }
        i += j;

        RSAPrivateWrapper privKey(Base64Wrapper::decode(bytesToString(_privateKey)));
        ServerMsg::printMsg(currMessage, privKey, _clientList);
    }
    return res;
}

/**
 * @brief Sends a message and returns the corresponding byte vector.
 * @return A vector of bytes representing the send message command.
 */
std::vector<CryptoPP::byte> ClientCmd::sendMsg()
{
    std::vector<CryptoPP::byte> res;

    std::ifstream userFile(INFO_FILE_NAME);
    if (!userFile.is_open())
    {
        std::cout << "FILE DIDNT OPEN\n";
        _code = getBytesAsCryptoPP(NOT_REGISTERED, CODE_SIZE);
        _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
        mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
        return res;
    }

    _cid = getCidOfInfoFile(userFile);

    std::string otherName;

    std::cout << "Enter Destination User Name: ";
    std::getline(std::cin, otherName);

    if (!otherName.empty() && otherName[otherName.length() - 1] == '\n')
    {
        otherName.pop_back();
    }

    if (otherName == bytesToString(_userName))
    {
        _code = getBytesAsCryptoPP(SAME_CLIENT_ERROR, CODE_SIZE);
        _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
        mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
        return res;
    }

    if (otherName.length() > NAME_SIZE)
    {
        otherName = otherName.substr(0, NAME_SIZE); // Truncate input
    }

    if (nameExists(otherName))
    {
        _otherCid = _clientList.left.find(otherName)->second.getCid();
    }
    else
    {
        _code = getBytesAsCryptoPP(NO_CLIENT_NAME_ERROR, CODE_SIZE);
        _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
        mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
        return res;
    }

    const int msgType = bytesToType<int>(_msgType);

    std::string curUsrName;

    /**
     * @brief Handles the case when the message type is SYM_KEY_REQ.
     * @return A vector of bytes representing the request for a symmetric key.
     */
    if (msgType == SYM_KEY_REQ)
    {
        for (const auto& pair : _clientList.right)
        {
            if (pair.first.getCid() == _cid)
            {
                curUsrName = pair.second;
            }
        }

        const auto curClient = _clientList.left.find(curUsrName);
        const auto destClient = _clientList.left.find(otherName);
        Client curClientCopy = curClient->second;
        Client destClientCopy = destClient->second;
        // need to check if public key exists
        if (!destClientCopy.pubKeyExists())
        {
            std::cout << "user didn't ask for public key of user requested yet";
            _code = getBytesAsCryptoPP(NO_PUBLIC_KEY_ERROR, CODE_SIZE);
            _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
            mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
            return res;
        }

        // send to server
        std::cout << bytesToType<int>(_code) << "\n";

        std::vector<CryptoPP::byte> msgToSend{};
        _contentSize = getBytesAsCryptoPP(0, MSG_SZ_SIZE);
        _payloadSize = getBytesAsCryptoPP(_otherCid.size() + _msgType.size() + _contentSize.size() + _msgContent.size(), PAYLOAD_SZ_SIZE);
        mergeVector<CryptoPP::byte>(msgToSend, {_cid,_version, _code, _payloadSize, _otherCid, _msgType, _contentSize});
        _serverHandler.sendMessage(msgToSend);
        ServerMsg msgToReceive(_serverHandler.receiveMessage());

        if (bytesToType<int>(msgToReceive.getCode()) == SERVER_ERROR)
        {
            _code = getBytesAsCryptoPP(SERVER_ERROR, CODE_SIZE);
            _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
            mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
            return res;
        }
        _payloadSize = msgToReceive.getPayloadSizeVec();
        _payload = msgToReceive.getPayload();
        mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize, _payload});
        return res;
    }
    /**
     * @brief Handles the case when the message type is SYM_KEY_SEND.
     * @return A vector of bytes representing the sending of a symmetric key.
     */
    if (msgType == SYM_KEY_SEND)
    {
        for (const auto& pair : _clientList.right)
        {
            if (pair.first.getCid() == _cid)
            {
                curUsrName = pair.second;
            }
        }

        const auto curClient = _clientList.left.find(curUsrName);
        const auto destClient = _clientList.left.find(otherName);
        Client curClientCopy = curClient->second;
        Client destClientCopy = destClient->second;
        if (destClientCopy.pubKeyExists())
        {
            AESWrapper createSymKey{};
            curClientCopy.setSymKey(stringToBytes(std::string(reinterpret_cast<const char*>(createSymKey.getKey()))));

            _clientList.insert({curUsrName, curClientCopy});

            std::vector<CryptoPP::byte> destClientPubKey = destClientCopy.getPublicKey();
            std::vector<CryptoPP::byte> curClientSymKey = curClientCopy.getSymKey();

            AESWrapper symKey(reinterpret_cast<const unsigned char*>(bytesToString(curClientSymKey).c_str()),
                              SYM_KEY_SIZE);
            std::string encryptedSymKey = symKey.encrypt(bytesToString(destClientPubKey).c_str(), SYM_KEY_SIZE);

            std::vector<CryptoPP::byte> msgToSend{};

            _msgContent = stringToBytes(encryptedSymKey);
            _contentSize = getBytesAsCryptoPP(_msgContent.size(), MSG_SZ_SIZE);
            _payloadSize = getBytesAsCryptoPP(_otherCid.size() + _msgType.size() + _contentSize.size() + _msgContent.size(), PAYLOAD_SZ_SIZE);
            mergeVector<CryptoPP::byte>(msgToSend, {_cid,_version, _code, _payloadSize, _otherCid, _msgType, _contentSize,_msgContent});
            _serverHandler.sendMessage(msgToSend);
            ServerMsg msgToReceive(_serverHandler.receiveMessage());

            if (bytesToType<int>(msgToReceive.getCode()) == SERVER_ERROR)
            {
                _code = getBytesAsCryptoPP(SERVER_ERROR, CODE_SIZE);
                _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
                mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
                return res;
            }

            _payloadSize = msgToReceive.getPayloadSizeVec();
            _payload = msgToReceive.getPayload();
            mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize, _payload});
            return res;
        }
        std::cout << "user didn't ask for public key of user requested yet";
        _code = getBytesAsCryptoPP(NO_PUBLIC_KEY_ERROR, CODE_SIZE);
        _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
        mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
        return res;
    }

    /**
     * @brief Handles the case when the message type is TEXT_SEND.
     * @return A vector of bytes representing the sending of a text message.
     */
    if (msgType == TEXT_SEND)
    {
        for (const auto& pair : _clientList.right)
        {
            if (pair.first.getCid() == _cid)
            {
                curUsrName = pair.second;
            }
        }

        const auto curClient = _clientList.left.find(curUsrName);
        const auto destClient = _clientList.left.find(otherName);
        Client curClientCopy = curClient->second;
        Client destClientCopy = destClient->second;

        if (!destClientCopy.pubKeyExists())
        {
            std::cout << "user didn't ask for public key of user requested yet";
            _code = getBytesAsCryptoPP(NO_PUBLIC_KEY_ERROR, CODE_SIZE);
            _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
            mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
            return res;
        }

        if (!destClient->second.symKeyExists())
        {
            std::cout << "user didn't ask for sym key";
            _code = getBytesAsCryptoPP(NO_SYM_KEY_ERROR, CODE_SIZE);
            _payloadSize = getBytesAsCryptoPP(0, PAYLOAD_SZ_SIZE);
            mergeVector<CryptoPP::byte>(res, {_version, _code, _payloadSize});
            return res;
        }

        std::string text;

        std::cout << "Enter Message to send:";
        std::getline(std::cin, text);

        AESWrapper symKey(reinterpret_cast<const unsigned char*>(bytesToString(destClient->second.getSymKey()).c_str()),
                          SYM_KEY_SIZE);

        std::string encryptedText = symKey.encrypt(text.c_str(), text.size());

        _msgContent = stringToBytes(encryptedText);
        _contentSize = getBytesAsCryptoPP(_msgContent.size(), MSG_SZ_SIZE);
        _payloadSize = getBytesAsCryptoPP(_otherCid.size() + _msgType.size() + _contentSize.size() + _msgContent.size(),
                                          PAYLOAD_SZ_SIZE);
        mergeVector<CryptoPP::byte>(
            res, {_version, _code, _payloadSize, _otherCid, _msgType, _contentSize, _msgContent});
    }

    /**
     * @brief Placeholder for file send implementation.
     * @return An empty vector of bytes.
     */
    // if I want to implement file send;
    return {};
}

/**
 * @brief Checks if a name exists in the client list.
 * @param name The name to check.
 * @return True if the name exists, false otherwise.
 */
bool ClientCmd::nameExists(const std::string& name)
{
    return _clientList.left.find(name) != _clientList.left.end();
}
