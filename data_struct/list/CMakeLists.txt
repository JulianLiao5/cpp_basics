cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O3 -std=c++11")

add_executable(test_max_min test_max_min.cpp)

add_executable(test_max_min2 test_max_min2.cpp)
