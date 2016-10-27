add_library(Storage storage.cpp include/storage.h)
target_link_libraries(Storage ${OPENSSL_LIBRARIES})

add_library(SafeString safe_string.cpp include/safe_string.h)
target_link_libraries(SafeString ${OPENSSL_LIBRARIES})

add_library(Entry entry.cpp include/entry.h)

target_link_libraries(Storage SafeString)
target_link_libraries(Storage Entry)
target_link_libraries(Entry SafeString)

add_executable(CPP-Pass main.cpp include/disk_storage.h disk_storage.cpp)
target_link_libraries(CPP-Pass Storage)
target_link_libraries(CPP-Pass SafeString)