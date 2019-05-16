

CMake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)

add_executable(test_atomic test_atomic_load_store.cc)
target_link_libraries(test_atomic pthread)
