cmake_minimum_required(VERSION 2.8)
project(CPP-Pass)

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

add_subdirectory(src)

enable_testing()
add_subdirectory(test)
