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

template <typename T, typename... Args>
void mergeVector(const std::vector<T>& res, const std::vector<T>& vec1, const Args&... rest) {
    res.insert(res.end(), vec1.begin(), vec1.end());
    if (sizeof...(rest) > 0) {
        mergeVector(res, rest...);
    }
}

std::string bytesToString(std::vector<CryptoPP::byte> bytes) {
    std::string result;
    for (int i = 0; i < bytes.size(); i++) {
        result += std::to_string(bytes[i]);
    }
    return result;
}

int sumVector(const std::vector<CryptoPP::byte>& bytes) {
    int result = 0;
    for (int i = 0; i < bytes.size(); i++) {
        result += bytes[i];
    }
    return result;
}

unsigned char extractByte(const int value, const int byteIndex) {
    return static_cast<unsigned char>((value >> (byteIndex * 8)) & 0xff);
}