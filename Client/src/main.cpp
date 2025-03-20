//
// Created by yairtop1 on 2/27/25.
//
#include <iostream>
#include "utils.h"
#include "parseCmd.h"
#include "client.h"

/**
 * @brief Entry point for the MessageU client application.
 * 
 * This function initializes the server handler and client command handler,
 * displays a menu for user commands, and processes the commands accordingly.
 * 
 * @return int Exit status of the application.
 */
int main() {
    // Read server information from the server info file
    const std::vector<std::string> serverInfo = readServerInfo(SERVER_INFO_FILE);
    if (serverInfo.empty()) {
        std::cerr << "\nError reading server info\n";
        exit(EXIT_FAILURE);
    }

    std::string cmd;
    // Initialize the server handler with the server IP and port
    ServerHandler server_handler(serverInfo[0], std::stoi(serverInfo[1]));
    // Initialize the client command handler with the server handler
    ClientCmd client(server_handler);

    bool gottenClientsList = false;

    // Display the menu and process user commands
    while (cmd != "0") {
        std::cout << "MessageU client at your service.\n\n";
        std::cout << "110) Register\n";
        std::cout << "120) Request for clients list\n";
        std::cout << "130) Request for public key\n";
        std::cout << "140) Request for waiting messages\n";
        std::cout << "150) Send a text message\n";
        std::cout << "151) Send a request for symmetric key\n";
        std::cout << "152) Send your symmetric key\n";
        std::cout << "153) Send a file\n";
        std::cout << "0) Exit client\n";
        std::cout << "? ";

        // Get user input
        std::getline(std::cin, cmd);

        // Parse the command and get the response from the server
        std::vector<CryptoPP::byte> retValue = client.parseCommand(cmd,gottenClientsList);
        ServerMsg ans(retValue);

        // Print the response code
        std::cout << bytesToType<int>(ans.getCode()) << "\n";

        if (bytesToType<int>(ans.getCode()) == 2101)
        {
            gottenClientsList = true;
        }
        // Check for errors in the response and print them if any
        if (ServerMsg::errorsExist(ans)) {
            ServerMsg::printError(ans);
        }

        // Reset the command
        cmd = "";
    }
}