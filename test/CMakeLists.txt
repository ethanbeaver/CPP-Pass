include_directories(../src)
include_directories(../src/include)

add_executable(TestStorage test_storage.cpp)
target_link_libraries(TestStorage Storage)
add_test(NAME TestStorage COMMAND TestStorage)

add_executable(TestSafeString test_safe_string.cpp)
target_link_libraries(TestSafeString SafeString)
#target_link_libraries(TestSafeString ssl crypto)
add_test(NAME TestSafeString COMMAND TestSafeString)