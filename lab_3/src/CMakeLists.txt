cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(lab_3)
set(CMAKE_C_FLAGS "-pthread")
add_executable(lab ./src/main.c)