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