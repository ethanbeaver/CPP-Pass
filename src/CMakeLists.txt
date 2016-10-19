add_library(Storage storage.cpp include/storage.h)

add_executable(CPP-Pass main.cpp)
target_link_libraries(CPP-Pass Storage)