cmake_minimum_required(VERSION 3.1)
project(linux_practice)

set(CMAKE_CXX_STANDARD 17)

add_executable(time time.cpp)
add_executable(signal signal.cpp)
add_executable(fork fork.cpp)
add_executable(shared_memory shared_memory.cpp)
