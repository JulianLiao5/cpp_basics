cmake_minimum_required(VERSION 3.5)
project(perception_sdk)

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_BUILD_TYPE debug)

add_library(pi_universal_perception STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/pi_perception.cpp)

target_link_libraries(pi_universal_perception PUBLIC piauto_perception)
message(STATUS "EIGEN_INCLUDE_DIR: ${EIGEN_INCLUDE_DIR}")
target_include_directories(pi_universal_perception PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/../fusion/include
        )

############################# Below is the sample cmake for pi_universal_perception lib usage

add_executable(perception_sample test/perception_sample.cpp)

target_link_libraries(perception_sample
        pi_universal_perception)
