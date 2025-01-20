#include <iostream>
#include "utils.h"
#include "parseCmd.h"

int main() {
    const std::vector<std::string> serverInfo = readServerInfo(SERVER_INFO_FILE);
    if (serverInfo.empty()) {
        std::cerr << "\nError reading server info\n";
        exit(EXIT_FAILURE);
    }

    std::string cmd;
    ServerHandler server_handler(serverInfo[0],std::stoi(serverInfo[1]));
    ClientCmd client(server_handler);
    while (cmd != "0") {
        std::cout << "MessageU client at your service.\n\n";
        std::cout << "110) Register\n";
        std::cout << "120) Request for clients list\n";
        std::cout << "130) Request for public key\n";
        std::cout << "140) Request for waiting messages\n";
        std::cout << "150) Send a text message\n";
        std::cout << "151) Send a request for symmetric key\n";
        std::cout << "152) Request for messages without waiting messages\n";
        std::cout << "153) Send a file\n";
        std::cout << "0) Exit client\n";
        std::cout << "? ";

        std::getline(std::cin,cmd);

        std::vector<CryptoPP::byte> retValue = client.parseCommand(cmd);
        ServerMsg ans(retValue);
        if (ServerMsg::errorsExist(ans)) {
            ServerMsg::printError(ans);
        }
    }


}