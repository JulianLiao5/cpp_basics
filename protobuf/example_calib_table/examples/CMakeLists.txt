cmake_minimum_required(VERSION 3.5)
project(protobuf-cpp-examples)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O3 -std=c++11")

# Set examples list
set(examples proto_from_file)

# Make sure the generated proto header and source files are visible to the other projects
include_directories(${CMAKE_BINARY_DIRS}/protos)

foreach(example ${examples})
    add_subdirectory(${example})
endforeach()
