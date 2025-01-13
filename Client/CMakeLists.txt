# CMakeLists.txt file

cmake_minimum_required(VERSION 3.30)
project(Client)

set(CMAKE_CXX_STANDARD 11)

# Ensure VCPKG is properly set
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}\\scripts\\buildsystems\\vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
else()
    message(FATAL_ERROR "VCPKG_ROOT is not set!")
endif()

# Set CMake prefix path for vcpkg
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};$ENV{VCPKG_ROOT}\\installed\\x64-windows")

# Add executable
add_executable(Client src/client.cpp)

# Find and link CryptoPP
find_package(cryptopp CONFIG REQUIRED)
if (cryptopp_FOUND)
    target_link_libraries(Client PRIVATE cryptopp::cryptopp)
else()
    message(FATAL_ERROR "CryptoPP not found!")
endif()

# Find and link boost
find_package(Boost REQUIRED COMPONENTS asio)

if (Boost_FOUND)
    target_include_directories(Client PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(Client PRIVATE Boost::asio)

else()
    message(FATAL_ERROR "boost not found!")
endif()


# Enable verbose output for debugging
set(CMAKE_VERBOSE_MAKEFILE ON)
