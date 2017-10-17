cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O3 -std=c++11")

add_executable(test_condition_variable test_condition_variable.cpp)
target_link_libraries(test_condition_variable pthread)

add_executable(test_high_resolution_clock test_high_resolution_clock.cpp)
