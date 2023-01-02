# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
add_executable(trace trace.c)
target_link_libraries(trace PRIVATE b_stacktrace::b_stacktrace)

add_executable(trace-cpp trace.cpp)
target_link_libraries(trace-cpp PRIVATE b_stacktrace::b_stacktrace)

add_executable(crash crash.c)
target_link_libraries(crash PRIVATE b_stacktrace::b_stacktrace)
