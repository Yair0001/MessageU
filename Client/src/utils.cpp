#include "utils.h"

/**
 * @brief Splits a string by a delimiter.
 * @param s The string to split.
 * @param del The delimiter character.
 * @return A vector of split strings.
 */
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

/**
 * @brief Reads server information from a file.
 * @param path The path to the server info file.
 * @return A vector of strings containing the server information.
 */
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

/**
 * @brief Prints a message from a vector of bytes in hexadecimal format.
 * @param msg The vector of bytes representing the message.
 */
void printMsg(const std::vector<CryptoPP::byte>& msg) {
    for (const auto& byte : msg) {
        std::cout << std::hex << std::uppercase << static_cast<int>(byte);
    }
    std::cout << std::dec << std::endl;
}

/**
 * @brief Prints a message string from a vector of bytes.
 * @param msg The vector of bytes representing the message string.
 */
void printMsgString(const std::vector<CryptoPP::byte>& msg) {
    for (const auto& byte : msg) {
        std::cout << static_cast<char>(byte);
    }
    std::cout << std::endl;
}

/**
 * @brief Converts a vector of bytes to a string.
 * @param bytes The vector of bytes.
 * @return The resulting string.
 */
std::string bytesToString(const std::vector<CryptoPP::byte> &bytes) {
    return std::string(bytes.begin(), bytes.end());
}

/**
 * @brief Converts a vector of bytes to a hexadecimal string.
 * @param bytes The vector of bytes.
 * @return The resulting hexadecimal string.
 */
std::string bytesToHex(const std::vector<CryptoPP::byte>& bytes) {
    std::stringstream hex_stream;
    hex_stream << std::hex << std::setfill('0'); // Set fill for leading zeros
    for (const auto byte : bytes) {
        hex_stream << std::setw(2) << static_cast<int>(byte);
    }
    return hex_stream.str();
}

/**
 * @brief Sums the values in a vector of bytes.
 * @param bytes The vector of bytes.
 * @return The sum of the byte values.
 */
int sumVector(const std::vector<CryptoPP::byte>& bytes) {
    int result = 0;
    for (const unsigned char byte : bytes) {
        result += byte;
    }
    return result;
}

/**
 * @brief Extracts a subvector of bytes from a given vector.
 * @param bytes The original vector of bytes.
 * @param start The starting index.
 * @param end The ending index.
 * @return The resulting subvector of bytes.
 */
std::vector<CryptoPP::byte> numOfBytes(const std::vector<CryptoPP::byte>& bytes, const int start, const int end) {
    std::vector<CryptoPP::byte> result;
    for (int i = start; i < end; i++) {
        result.push_back(bytes[i]);
    }
    return result;
}

/**
 * @brief Converts a string to a vector of bytes.
 * @param str The string to convert.
 * @return The resulting vector of bytes.
 */
std::vector<CryptoPP::byte> stringToBytes(const std::string& str) {
    std::vector<CryptoPP::byte> result;
    for (int i = 0; i < str.size(); i++) {
        result.push_back(str[i]);
    }
    return result;
}

/**
 * @brief Reads the contents of a file into a vector of strings.
 * @param file The input file stream.
 * @return A vector of strings containing the file contents.
 */
std::vector<std::string> getFileContents(std::ifstream& file) {
    std::vector<std::string> outputs;
    std::string line;
    while (std::getline(file, line)) {
        outputs.push_back(line);
    }
    return outputs;
}

/**
 * @brief Converts a hexadecimal string to a vector of bytes.
 * @param hexString The hexadecimal string.
 * @return The resulting vector of bytes.
 */
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

/**
 * @brief Retrieves the client ID from an info file.
 * @param file The input file stream.
 * @return The client ID as a vector of bytes.
 */
std::vector<CryptoPP::byte> getCidOfInfoFile(std::ifstream& file)
{
    const std::vector<std::string> infoFileTokens = getFileContents(file);
    std::vector<CryptoPP::byte> cid = hexStrToBytes(infoFileTokens[1]);
    return cid;
}

std::vector<CryptoPP::byte> getPrivKeyOfInfoFile(std::ifstream& file)
{
    const std::vector<std::string> infoFileTokens = getFileContents(file);
    std::vector<CryptoPP::byte> privKey = hexStrToBytes(infoFileTokens[2]);
    return privKey;
}

/**
 * @brief Safely converts a string to an integer.
 * @param command The string to convert.
 * @return The resulting integer.
 */
int safe_stoi(const std::string& command) {
    try {
        size_t pos;
        int result = std::stoi(command, &pos);

        // Ensure the entire string was converted (no extra characters)
        if (pos != command.length()) {
            throw std::invalid_argument("Extra characters in input");
        }

        return result;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: Not a valid integer!\n";
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: Number out of range!\n";
    }
    return 0; // Return a default value (modify as needed)
}