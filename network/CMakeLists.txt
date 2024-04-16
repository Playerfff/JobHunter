cmake_minimum_required(VERSION 3.27)
project(test_framework)

set(CMAKE_CXX_STANDARD 17)

add_executable(socket_server socket_server.cpp)
add_executable(socket_client socket_client.cpp)
add_executable(linux_read_file linux_read_file.cpp)
add_executable(linux_write_file linux_write_file.cpp)
