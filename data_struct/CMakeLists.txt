cmake_minimum_required(VERSION 3.5)

project(cpp_data_struct)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O3 -std=c++11")

add_executable(test_data_struct test_data_struct.cc)

add_executable(test_vector test_vector.cc)

add_executable(test_map test_map.cc)
