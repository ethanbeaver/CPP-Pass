include_directories(../src)
include_directories(../src/include)

add_executable(TestStorage test_storage.cpp)
target_link_libraries(TestStorage Storage)
add_test(NAME TestStorage COMMAND TestStorage)