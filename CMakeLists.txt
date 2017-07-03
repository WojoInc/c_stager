cmake_minimum_required(VERSION 3.7)
project(c_stager)

set(CMAKE_C_STANDARD 99)

set(SERVER_SOURCE server.c)
set(CLIENT_SOURCE client.c)
add_executable(server ${SERVER_SOURCE})
add_executable(client ${CLIENT_SOURCE})
add_executable(payload test_payload.c)