cmake_minimum_required(VERSION 2.8)
project(CPP-Pass)

# Enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Import OpenSSL
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

# Import TCLAP
find_package(TCLAP)
include_directories(${TCLAP_INCLUDE_DIRS})

# Add sources
add_subdirectory(src)

# Add tests
if (EXISTS test)
    enable_testing()
    add_subdirectory(test)
    MESSAGE("Enabled testing!")
else ()
    MESSAGE("Testing has been disabled.")
endif ()