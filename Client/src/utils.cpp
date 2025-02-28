#include "utils.h"

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

void printMsg(const std::vector<CryptoPP::byte>& msg) {
    for (const auto& byte : msg) {
        std::cout << std::hex << std::uppercase << static_cast<int>(byte);
    }
    std::cout << std::dec << std::endl;
}

void printMsgString(const std::vector<CryptoPP::byte>& msg) {
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
    hex_stream << std::hex << std::setfill('0'); // Set fill for leading zeros
    for (const auto byte : bytes) {
        hex_stream << std::setw(2) << static_cast<int>(byte);
    }
    return hex_stream.str();
}

int sumVector(const std::vector<CryptoPP::byte>& bytes) {
    int result = 0;
    for (const unsigned char byte : bytes) {
        result += byte;
    }
    return result;
}

std::vector<CryptoPP::byte> numOfBytes(const std::vector<CryptoPP::byte>& bytes, const int start, const int end) {
    std::vector<CryptoPP::byte> result;
    for (int i = start; i < end; i++) {
        result.push_back(bytes[i]);
    }
    return result;
}

std::vector<CryptoPP::byte> stringToBytes(const std::string& str) {
    std::vector<CryptoPP::byte> result;
    for (int i = 0; i < str.size(); i++) {
        result.push_back(str[i]);
    }
    return result;
}

std::vector<std::string> getFileContents(std::ifstream& file) {
    std::vector<std::string> outputs;
    std::string line;
    while (std::getline(file, line)) {
        outputs.push_back(line);
    }
    return outputs;
}

std::vector<CryptoPP::byte> hexStrToBytes(const std::string& hexString) {
    // Ensure the input string length is even
    if (hexString.length() % 2 != 0) {
        throw std::invalid_argument("Hex string must have an even length");
    }

    std::vector<CryptoPP::byte> byteVector;
    byteVector.reserve(hexString.length() / 2);

    for (size_t i = 0; i < hexString.length(); i += 2) {
        // Convert each hex pair to a CryptoPP::byte
        std::string byteString = hexString.substr(i, 2);
        auto byte = static_cast<CryptoPP::byte>(std::stoul(byteString, nullptr, 16));
        byteVector.push_back(byte);
    }

    return byteVector;
}

std::vector<CryptoPP::byte> getCidOfInfoFile(std::ifstream& file)
{
    const std::vector<std::string> infoFileTokens = getFileContents(file);
    std::vector<CryptoPP::byte> cid = hexStrToBytes(infoFileTokens[1]);
    return cid;
}