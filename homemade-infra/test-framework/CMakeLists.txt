cmake_minimum_required(VERSION 3.27)
project(test_framework)

set(CMAKE_CXX_STANDARD 17)

add_executable(test_framework test.cpp unittest.cpp)
