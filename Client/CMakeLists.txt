cmake_minimum_required(VERSION 3.30)
project(Client)

set(CMAKE_CXX_STANDARD 11)

# Ensure VCPKG is properly set
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
else()
    message(FATAL_ERROR "VCPKG_ROOT is not set!")
endif()


# Optional: Set architecture-specific flags (for MinGW-w64)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mrdrnd")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")

# Add executable
add_executable(Client
        src/client.cpp
        include/client.h
        src/utils.cpp
        include/utils.h
        src/parseCmd.cpp
        include/parseCmd.h
        src/ServerHandler.cpp
        include/ServerHandler.h
        src/ServerMsg.cpp
        include/ServerMsg.h)


target_include_directories(Client PRIVATE "${CMAKE_SOURCE_DIR}/include")


# Find and link CryptoPP
find_package(cryptopp CONFIG REQUIRED)
if(cryptopp_FOUND)
    target_link_libraries(Client PRIVATE cryptopp::cryptopp)
else()
    message(FATAL_ERROR "CryptoPP not found!")
endif()

# Find and link boost
find_package(Boost REQUIRED COMPONENTS asio)
if(Boost_FOUND)
    target_include_directories(Client PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(Client PRIVATE Boost::asio)
else()
    message(FATAL_ERROR "Boost not found!")
endif()

# Link to ws2_32 for Windows
if(WIN32)
    target_link_libraries(Client PRIVATE ws2_32)
endif()

# Enable verbose output for debugging
set(CMAKE_VERBOSE_MAKEFILE ON)
