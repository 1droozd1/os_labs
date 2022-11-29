cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(lab_4)

set(CMAKE_CXX_FLAGS "-pthread")
add_executable(child_process ./src/child_process.c)
add_executable(lab4 ./src/lab4.c)