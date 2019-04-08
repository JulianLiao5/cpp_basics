cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O3 -std=c++11")

find_package(Boost REQUIRED)
message(STATUS "boost include dirs: " ${Boost_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
message(STATUS "boost libraries dirs: " ${Boost_LIBRARIES})

add_executable(test_boost ./test_boost.cpp)
target_link_libraries(test_boost ${Boost_LIBRARIES})
