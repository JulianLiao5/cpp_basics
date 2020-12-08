cmake_minimum_required(VERSION 3.5)
project(sensors_perception VERSION 0.1)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../testA_client testA_client)
message(STATUS "+++++++ build testA_client")

set(EXT_LIBS
        testA_sdk
        )

set(SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/fusion/src/fusion_multi_sensors.cc
        )

include_directories((${CMAKE_CURRENT_BINARY_DIR}))

add_library(piauto_perception OBJECT ${SOURCE_FILES})
# target_compile_definitions(piauto_perception PUBLIC $<TARGET_PROPERTY:pi_localization_sdk,pi_localization_sdk_def>)
# target_compile_definitions(piauto_perception INTERFACE $<TARGET_PROPERTY:pi_localization_sdk,INTERFACE_COMPILE_DEFINITIONS>)
target_link_libraries(piauto_perception PUBLIC ${EXT_LIBS})
target_include_directories(piauto_perception PUBLIC
        ./fusion/include/
        ./sdk_source/include
        )

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sdk_source pi_sdk)

#[[
add_executable(radar_barrier_test test/src/radar_barrier_test.cc)
target_link_libraries(radar_barrier_test
        piauto_perception
        )

add_executable(radars_ui_test test/src/radars_ui_test.cc)
target_link_libraries(radars_ui_test
        piauto_perception
        grid
        gflags
        )

add_executable(radars_ui_test_2 test/src/radars_ui_test_2.cc)
target_link_libraries(radars_ui_test_2
        piauto_perception
        grid
        )

add_executable(sonars_ui_test test/src/sonars_ui_test.cc)
target_link_libraries(sonars_ui_test
        piauto_perception
        grid
        gflags
        )

add_executable(sonars_ui_test_2 test/src/sonars_ui_test_2.cc)
target_link_libraries(sonars_ui_test_2
        piauto_perception
        grid
        )

add_executable(fusion_barrier_test test/src/fusion_barrier_test.cc)
target_link_libraries(fusion_barrier_test
        piauto_perception
        gflags
        )
]]
