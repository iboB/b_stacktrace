# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 90)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(MSVC)
    set(DEV_WARNING_FLAGS "-D_CRT_SECURE_NO_WARNINGS=1 /Zc:__cplusplus /w34100 /permissive-")
else()
    set(DEV_WARNING_FLAGS "-Wall -Wextra -Werror=return-type")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEV_WARNING_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DEV_WARNING_FLAGS}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
