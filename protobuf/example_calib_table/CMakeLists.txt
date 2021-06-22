cmake_minimum_required(VERSION 3.5)
project(PROTOBUF_Examples VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O3 -std=c++11")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(protos)
add_subdirectory(examples)
