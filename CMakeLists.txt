cmake_minimum_required(VERSION 2.8)
project(CPP-Pass)

# Enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Import OpenSSL
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

# Add sources
add_subdirectory(src)

# Add tests
enable_testing()
add_subdirectory(test)
