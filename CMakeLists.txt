# Copyright 2023-2026 Braden Ganetsky
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required (VERSION 3.23)

project (k3_k3tchup
         VERSION 0
         LANGUAGES CXX)

set(K3TCHUP_BUILD_EXAMPLES OFF CACHE BOOL "Build examples")

set(K3TCHUP_CXX_STANDARD "" CACHE STRING "C++ standard to use")
set(K3TCHUP_CXX_WARNINGS "" CACHE STRING "C++ compiler warnings to enable")

if (K3TCHUP_CXX_STANDARD)
    include(cmake/standard/${K3TCHUP_CXX_STANDARD}.cmake)
endif()
if (K3TCHUP_CXX_WARNINGS)
    include(cmake/warnings/${K3TCHUP_CXX_WARNINGS}.cmake)
endif()

add_library(k3_k3tchup STATIC
    src/k3/k3tchup/runner.cpp
    src/k3/k3tchup/detail/context.cpp
    src/k3/k3tchup/detail/error.cpp
    src/k3/k3tchup/detail/fixture.cpp
    src/k3/k3tchup/detail/test.cpp
    src/k3/k3tchup/detail/result.cpp
)
add_library(k3::k3tchup ALIAS k3_k3tchup)
target_sources(k3_k3tchup PUBLIC FILE_SET headers TYPE HEADERS FILES
    include/k3/k3tchup.hpp
    include/k3/k3tchup_main.hpp
    include/k3/k3tchup/assert.hpp
    include/k3/k3tchup/fixture.hpp
    include/k3/k3tchup/runner.hpp
    include/k3/k3tchup/test.hpp
    include/k3/k3tchup/detail/context.hpp
    include/k3/k3tchup/detail/error.hpp
    include/k3/k3tchup/detail/fixture.hpp
    include/k3/k3tchup/detail/hash.hpp
    include/k3/k3tchup/detail/macros.hpp
    include/k3/k3tchup/detail/result.hpp
    include/k3/k3tchup/detail/stateful_tmp.hpp
    include/k3/k3tchup/detail/test.hpp
    include/k3/k3tchup/detail/void_assigner.hpp
)

target_include_directories(k3_k3tchup PUBLIC include)

enable_testing()
include(cmake/discover_tests.cmake)

if (K3TCHUP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
