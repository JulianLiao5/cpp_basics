

CMake_minimum_required(VERSION 3.5)

project(mutex_test)

set(CMAKE_CXX_STANDARD 14)


add_executable(test_cv_1 test_condition_variable_1.cc)
target_link_libraries(test_cv_1 pthread)

add_executable(test_cv_2 test_condition_variable_2.cc)
target_link_libraries(test_cv_2 pthread)

add_executable(test_locks test_locks.cc)
target_link_libraries(test_locks pthread)
