cmake_minimum_required(VERSION 2.8)
set(CMAKE_VERBOSE_MAKEFILE 1)

# NOTE: specify google test directory path
include_directories(./googletest/googletest/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/googletest/build/googlemock/gtest/)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wextra -std=c++11")

add_executable(vector3_test test_vector3.cpp)
target_link_libraries(vector3_test gtest_main gtest pthread)

add_executable(vector3_test_et test_vector3_et.cpp)
target_link_libraries(vector3_test_et gtest_main gtest pthread)

add_executable(vector2_test test_vector2.cpp)
target_link_libraries(vector2_test gtest_main gtest pthread)

add_executable(vector2_test_et test_vector2_et.cpp)
target_link_libraries(vector2_test_et gtest_main gtest pthread)

add_executable(tensor3_test test_tensor3.cpp)
target_link_libraries(tensor3_test gtest_main gtest pthread)

add_executable(tensor2_test test_tensor2.cpp)
target_link_libraries(tensor2_test gtest_main gtest pthread)

add_executable(f_decomposer_test test_f_decomposer.cpp)
target_link_libraries(f_decomposer_test gtest_main gtest pthread)

add_executable(boundary_test test_boundary.cpp)
target_link_libraries(boundary_test gtest_main gtest pthread)

