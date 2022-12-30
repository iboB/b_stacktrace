# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.10)

project(b_stacktrace-example)

add_executable(trace trace.c)
target_link_libraries(trace PUBLIC
    ${CMAKE_DL_LIBS}
)

add_executable(trace-cpp trace.cpp)
target_link_libraries(trace-cpp PUBLIC
    ${CMAKE_DL_LIBS}
)

add_executable(crash crash.c)
target_link_libraries(crash PUBLIC
    ${CMAKE_DL_LIBS}
)
