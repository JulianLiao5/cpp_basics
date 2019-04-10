cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O3 -std=c++11")

find_package(Boost REQUIRED)
message(STATUS "boost include dirs: " ${Boost_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
message(STATUS "boost libraries dirs: " ${Boost_LIBRARIES})

add_executable(test_boost ./test_boost.cpp)
target_link_libraries(test_boost ${Boost_LIBRARIES})

add_executable(test_boost_2 ./test_boost_2.cpp)
target_link_libraries(test_boost_2 ${Boost_LIBRARIES})

add_executable(test_boost_3 ./test_boost_3.cpp)
target_link_libraries(test_boost_3 ${Boost_LIBRARIES})

add_executable(test_boost_4 ./test_boost_4.cpp)
target_link_libraries(test_boost_4 ${Boost_LIBRARIES})

add_executable(test_connection ./connection/test_connection.cpp)
target_link_libraries(test_connection ${Boost_LIBRARIES})

add_executable(test_connection_2 ./connection/test_connection_2.cpp)
target_link_libraries(test_connection_2 ${Boost_LIBRARIES})

add_executable(test_connection_3 ./connection/test_connection_3.cpp)
target_link_libraries(test_connection_3 ${Boost_LIBRARIES})

add_executable(test_second_boost ./second_stage/test_second_boost.cpp)
target_link_libraries(test_second_boost ${Boost_LIBRARIES})

add_executable(doc_view ./examples/doc_view.cpp)
target_link_libraries(doc_view ${Boost_LIBRARIES})

add_executable(origin_1_58 ./examples/origin_doc_view_1_58_0.cpp)
target_link_libraries(origin_1_58 ${Boost_LIBRARIES})
