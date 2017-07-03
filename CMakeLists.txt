cmake_minimum_required(VERSION 3.7)
project(c_stager)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES server.c)
add_executable(c_stager ${SOURCE_FILES})