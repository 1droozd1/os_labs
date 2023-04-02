cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(lab6 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-lzmq")

add_executable(client ./src/client.cpp ./src/net_realization.hpp ./src/node_tree.hpp)
add_executable(server ./src/server.cpp ./src/net_realization.hpp ./src/node_tree.hpp)

target_link_libraries(client zmq)
target_link_libraries(server zmq)
