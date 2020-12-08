cmake_minimum_required(VERSION 3.0)
project(testA)

set(CMAKE_CXX_STANDARD 14)

add_library(testA
    src/testA.cpp
    )
target_include_directories(testA PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(testA)

add_executable(testA_sample
        src/testA_sample.cpp
        )

target_link_libraries(testA_sample PUBLIC
        testA
        )
