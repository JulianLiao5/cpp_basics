cmake_minimum_required(VERSION 3.5)
project(cpp_file_stream)

find_package(Glog REQUIRED)
include_directories(${GLOG_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O3 -std=c++11")

add_executable(test_fstream ./test_fstream.cc)
target_link_libraries(test_fstream glog)
