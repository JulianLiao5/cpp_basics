cmake_minimum_required(VERSION 3.5)
project(cpp_algorithm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O3 -std=c++11")

add_executable(lower_upper_example ./lower_upper_example.cc)
