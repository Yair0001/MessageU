#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iomanip>
#include <vector>
#include <fstream>
#include <cryptopp/trap.h>
#include <iostream>
#include <cstdint>
#include <sstream>

#define CLIENT_VERSION 2
#define INFO_FILE_NAME "my.info"
#define SERVER_INFO_FILE "../server.info"

#define BYTES_TILL_PAYLOAD (VERSION_SIZE+CODE_SIZE+PAYLOAD_SZ_SIZE)

/**
 * @enum ProtocolSizes
 * @brief Enumeration for various protocol sizes.
 */
enum ProtocolSizes{
    CLIENT_ID_SIZE=16,
    VERSION_SIZE=1,
    CODE_SIZE=2,
    PAYLOAD_SZ_SIZE=4,
    NAME_SIZE=255,
    PUBLIC_KEY_SIZE=160,
    SYM_KEY_SIZE=16,
    MSG_TYPE_SIZE=1,
    MSG_ID_SIZE=4,
    MSG_SZ_SIZE=4
};

/**
 * @enum ErrorCodes
 * @brief Enumeration for various error codes.
 */
enum ErrorCodes{
    EXIT=0x00,
    OK=0xFF,
    ALREADY_REGISTERED=0x01,
    NOT_REGISTERED=0x02,
    NO_CLIENTS_ERROR=0x03,
    INVALID_CID=0x04,
    SERVER_ERROR=9000,
    NO_CODE_ERROR=9001,
    NO_CLIENT_NAME_ERROR=9002,
    NO_MESSAGES_ERROR=9003,
    NO_PUBLIC_KEY_ERROR=9004,
    NO_SYM_KEY_ERROR=9005,
    SAME_CLIENT_ERROR=9006,
    NO_CLIENTS_LIST_ERROR=9007,
};

/**
 * @enum RequestCodes
 * @brief Enumeration for various request codes.
 */
enum RequestCodes {
    REGISTER_CODE=600,
    CLIENTS_LIST_CODE=601,
    PUBLIC_KEY_CODE=602,
    SEND_MSG_CODE=603,
    WAITING_LIST_CODE=604,
};

/**
 * @enum MessageTypes
 * @brief Enumeration for various message types.
 */
enum MessageTypes{
    SYM_KEY_REQ=1,
    SYM_KEY_SEND=2,
    TEXT_SEND=3,
    FILE_SEND=4,
};

/**
 * @enum AnswerCodes
 * @brief Enumeration for various answer codes.
 */
enum AnswerCodes {
    REGISTER_OK=2100,
    CLIENTS_LIST_OK=2101,
    PUBLIC_KEY_OK=2102,
    SEND_MSG_OK=2103,
    WAITING_LIST_OK=2104,
};

/**
 * @brief Splits a string by a delimiter.
 * @param s The string to split.
 * @param del The delimiter character.
 * @return A vector of split strings.
 */
std::vector<std::string> splitString(const std::string& s, char del);

/**
 * @brief Reads server information from a file.
 * @param path The path to the server info file.
 * @return A vector of strings containing the server information.
 */
std::vector<std::string> readServerInfo(const std::string& path);

/**
 * @brief Converts a vector of bytes to a string.
 * @param bytes The vector of bytes.
 * @return The resulting string.
 */
std::string bytesToString(const std::vector<CryptoPP::byte> &bytes);

/**
 * @brief Sums the values in a vector of bytes.
 * @param bytes The vector of bytes.
 * @return The sum of the byte values.
 */
int sumVector(const std::vector<CryptoPP::byte>& bytes);

/**
 * @brief Prints a message from a vector of bytes.
 * @param msg The vector of bytes representing the message.
 */
void printMsg(const std::vector<CryptoPP::byte>& msg);

/**
 * @brief Prints a message string from a vector of bytes.
 * @param msg The vector of bytes representing the message string.
 */
void printMsgString(const std::vector<CryptoPP::byte>& msg);

/**
 * @brief Converts a vector of bytes to a hexadecimal string.
 * @param bytes The vector of bytes.
 * @return The resulting hexadecimal string.
 */
std::string bytesToHex(const std::vector<CryptoPP::byte>& bytes);

/**
 * @brief Extracts a subvector of bytes from a given vector.
 * @param bytes The original vector of bytes.
 * @param start The starting index.
 * @param end The ending index.
 * @return The resulting subvector of bytes.
 */
std::vector<CryptoPP::byte> numOfBytes(const std::vector<CryptoPP::byte>& bytes, int start, int end);

/**
 * @brief Converts a string to a vector of bytes.
 * @param str The string to convert.
 * @return The resulting vector of bytes.
 */
std::vector<CryptoPP::byte> stringToBytes(const std::string& str);

/**
 * @brief Reads the contents of a file into a vector of strings.
 * @param file The input file stream.
 * @return A vector of strings containing the file contents.
 */
std::vector<std::string> getFileContents(std::ifstream& file);

/**
 * @brief Converts a hexadecimal string to a vector of bytes.
 * @param hexString The hexadecimal string.
 * @return The resulting vector of bytes.
 */
std::vector<CryptoPP::byte> hexStrToBytes(const std::string& hexString);

/**
 * @brief Retrieves the client ID from an info file.
 * @param file The input file stream.
 * @return The client ID as a vector of bytes.
 */
std::vector<CryptoPP::byte> getCidOfInfoFile(std::ifstream& file);

std::vector<CryptoPP::byte> getPrivKeyOfInfoFile(std::ifstream& file);

/**
 * @brief Safely converts a string to an integer.
 * @param command The string to convert.
 * @return The resulting integer.
 */
int safe_stoi(const std::string& command);

/**
 * @brief Merges multiple vectors into one.
 * @tparam T The type of the elements in the vectors.
 * @param res The resulting merged vector.
 * @param vecs The initializer list of vectors to merge.
 */
template <typename T>
void mergeVector(std::vector<T>& res, std::initializer_list<std::vector<T>> vecs) {
    for (const auto& vec : vecs) {
        res.insert(res.end(), vec.begin(), vec.end());
    }
}

/**
 * @brief Converts a value to a vector of CryptoPP bytes.
 * @tparam T The type of the value.
 * @param value The value to convert.
 * @param numOfBytes The number of bytes to extract.
 * @return The resulting vector of CryptoPP bytes.
 */
template <typename T>
std::vector<CryptoPP::byte> getBytesAsCryptoPP(T value, const size_t numOfBytes) {
    static_assert(std::is_integral<T>::value || std::is_enum<T>::value,
                  "T must be an integral or enum type");

    if (numOfBytes > sizeof(T)) {
        throw std::invalid_argument("numOfBytes exceeds the size of the input value type");
    }

    std::vector<CryptoPP::byte> bytes(numOfBytes);
    for (size_t i = 0; i < numOfBytes; ++i) {
        bytes[numOfBytes - 1 - i] = static_cast<CryptoPP::byte>((value >> (i * 8)) & 0xFF);
    }
    return bytes;
}

/**
 * @brief Converts a vector of CryptoPP bytes to a value.
 * @tparam T The type of the value.
 * @param bytes The vector of CryptoPP bytes.
 * @return The resulting value.
 */
template <typename T>
T bytesToType(const std::vector<CryptoPP::byte>& bytes) {
    static_assert(std::is_integral<T>::value, "T must be an integral type");

    if (bytes.size() > sizeof(T)) {
        throw std::invalid_argument("Byte vector size exceeds the size of the output type");
    }

    T value = 0;
    for (unsigned char byte : bytes) {
        value = (value << 8) | static_cast<T>(byte);
    }
    return value;
}

#endif //UTILS_H