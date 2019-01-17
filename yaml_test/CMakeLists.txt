cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O3 -std=c++11")

FIND_PACKAGE(yaml-cpp REQUIRED)

message("abc: " + ${YAML_CPP_INCLUDE_DIR})
include_directories(${YAML_CPP_INCLUDE_DIR})
add_executable(test_yaml src/test_yaml.cc)

target_link_libraries(test_yaml yaml-cpp)
