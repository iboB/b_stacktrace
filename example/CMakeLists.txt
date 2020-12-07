cmake_minimum_required(VERSION 3.10)

project(b_stacktrace-example)

set(CMAKE_C_STANDARD 89)

add_executable(trace trace.c)
add_executable(crash crash.c)
