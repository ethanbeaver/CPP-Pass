add_library(Storage storage.cpp include/storage.h)
add_library(SafeString safe_string.cpp include/safe_string.h)
target_link_libraries(SafeString ${OPENSSL_LIBRARIES})

add_executable(CPP-Pass main.cpp)
target_link_libraries(CPP-Pass Storage)
target_link_libraries(CPP-Pass SafeString)