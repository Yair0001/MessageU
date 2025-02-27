cmake_minimum_required(VERSION 3.30)
project(Client)

set(CMAKE_CXX_STANDARD 11)


# Add executable
add_executable(Client
        src/main.cpp
        src/client.cpp
        include/client.h
        src/utils.cpp
        include/utils.h
        src/parseCmd.cpp
        include/parseCmd.h
        src/ServerHandler.cpp
        include/ServerHandler.h
        src/ServerMsg.cpp
        include/ServerMsg.h
        src/AESWrapper.cpp
        include/AESWrapper.h
        src/RSAWrapper.cpp
        include/RSAWrapper.h
        src/Base64Wrapper.cpp
        include/Base64Wrapper.h)

target_compile_options(Client PRIVATE -mrdrnd)
include_directories(/usr/include)

target_link_libraries(Client PRIVATE cryptopp)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

target_link_libraries(Client PRIVATE
        Boost::headers
        Threads::Threads
)


target_include_directories(Client PRIVATE ${CMAKE_SOURCE_DIR}/include)
