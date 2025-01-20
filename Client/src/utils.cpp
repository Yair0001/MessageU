#include "utils.h"

#include <iostream>

std::vector<std::string> splitString(const std::string& s, const char del) {
    std::vector<std::string> result;
    std::string::size_type start = 0;
    std::string::size_type end = 0;
    while ((end = s.find(del, start)) != std::string::npos) {
        result.push_back(s.substr(start, end - start));
        start = end + 1;
    }
    result.push_back(s.substr(start));
    return result;
}

std::vector<std::string> readServerInfo(const std::string& path) {
    std::fstream file(path);
    std::vector<std::string> result;
    if (file.is_open()) {
        const auto fileContent = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
        result = splitString(fileContent, ':');
        return result;
    }
    return result;
}

void printMsg(std::vector<CryptoPP::byte> msg) {
    for (const auto& byte : msg) {
        std::cout << std::hex << std::uppercase << static_cast<int>(byte);
    }
    std::cout << std::dec << std::endl;
}

void printMsgString(std::vector<CryptoPP::byte> msg) {
    for (const auto& byte : msg) {
        std::cout << static_cast<char>(byte);
    }
    std::cout << std::endl;
}

std::string bytesToString(const std::vector<CryptoPP::byte> &bytes) {
    return std::string(bytes.begin(), bytes.end());
}

std::string bytesToHex(const std::vector<CryptoPP::byte>& bytes) {
    std::stringstream hex_stream;
    for (const auto byte : bytes) {
        hex_stream << std::hex << static_cast<int>(byte);
    }
    return hex_stream.str();
}

int sumVector(const std::vector<CryptoPP::byte>& bytes) {
    int result = 0;
    for (int i = 0; i < bytes.size(); i++) {
        result += bytes[i];
    }
    return result;
}

std::vector<CryptoPP::byte> numOfBytes(std::vector<CryptoPP::byte> bytes, int start, int end) {
    std::vector<CryptoPP::byte> result;
    for (int i = start; i < end; i++) {
        result.push_back(bytes[i]);
    }
    return result;
}
