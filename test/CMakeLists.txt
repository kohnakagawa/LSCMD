cmake_minimum_required(VERSION 2.8)
set(CMAKE_VERBOSE_MAKEFILE 1)

# NOTE: specify google test directory path
include_directories(./googletest/googletest/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)

include_directories(/usr/include/eigen3)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/googletest/build/googlemock/gtest/)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage --coverage")

set(GTEST_LINK_LIBS gtest_main gtest pthread)
set(GCOV_LINK_LIBS gcov)
set(LINK_LIBS ${GCOV_LINK_LIBS} ${GTEST_LINK_LIBS})

add_executable(vector3_test.out test_vector3.cpp)
target_link_libraries(vector3_test.out ${LINK_LIBS})

add_executable(vector2_test.out test_vector2.cpp)
target_link_libraries(vector2_test.out ${LINK_LIBS})

add_executable(tensor3_test.out test_tensor3.cpp)
target_link_libraries(tensor3_test.out ${LINK_LIBS})

add_executable(tensor2_test.out test_tensor2.cpp)
target_link_libraries(tensor2_test.out ${LINK_LIBS})

add_executable(f_decomposer_test.out test_f_decomposer.cpp)
target_link_libraries(f_decomposer_test.out ${LINK_LIBS})

add_executable(boundary_test.out test_boundary.cpp)
target_link_libraries(boundary_test.out ${LINK_LIBS})

add_executable(byte_utils_test.out test_byte_utils.cpp)
target_link_libraries(byte_utils_test.out ${LINK_LIBS})
