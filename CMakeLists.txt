# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.10)

project(b_stacktrace)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # dev_mode is used below to make life simpler for developers
    # it enables some configurations and the defaults for building tests and examples
    # which typically wouldn't be built if confy is a subdirectory of another project
    set(dev_mode ON)
else()
    set(dev_mode OFF)
endif()

option(B_STACKTRACE_BUILD_TESTS "b_stacktrace: build tests" ${dev_mode})
option(B_STACKTRACE_BUILD_EXAMPLES "b_stacktrace: build examples" ${dev_mode})
option(B_STACKTRACE_BUILD_SCRATCH "b_stacktrace: build scratch project for testing and experiments" ${dev_mode})

mark_as_advanced(B_STACKTRACE_BUILD_TESTS B_STACKTRACE_BUILD_EXAMPLES B_STACKTRACE_BUILD_SCRATCH)

if(dev_mode)
    include(./dev.cmake)
endif()

add_library(b_stacktrace INTERFACE)
add_library(b_stacktrace::b_stacktrace ALIAS b_stacktrace)
target_include_directories(b_stacktrace INTERFACE include)
target_link_libraries(b_stacktrace INTERFACE ${CMAKE_DL_LIBS})

if(B_STACKTRACE_BUILD_SCRATCH)
    add_subdirectory(scratch)
endif()

#~ if(B_STACKTRACE_BUILD_TESTS)
#~     enable_testing()
#~     add_subdirectory(test)
#~ endif()

if(B_STACKTRACE_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

