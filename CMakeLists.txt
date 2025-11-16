# Copyright 2023-2025 Braden Ganetsky
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required (VERSION 3.23)

project (k3_k3tchup
         VERSION 0
         LANGUAGES CXX)

set(K3TCHUP_CXX_STANDARD "detect" CACHE STRING "C++ standard to use")
set(K3TCHUP_CXX_WARNINGS "detect" CACHE STRING "C++ compiler warnings to enable")

if (K3TCHUP_CXX_STANDARD)
    include(cmake/standard/${K3TCHUP_CXX_STANDARD}.cmake)
endif()
if (K3TCHUP_CXX_WARNINGS)
    include(cmake/warnings/${K3TCHUP_CXX_WARNINGS}.cmake)
endif()

add_library(k3_k3tchup STATIC
    src/k3/k3tchup/error.cpp
    src/k3/k3tchup/fixture.cpp
    src/k3/k3tchup/result.cpp
    src/k3/k3tchup/runner.cpp
    src/k3/k3tchup/test.cpp
)
add_library(k3::k3tchup ALIAS k3_k3tchup)
target_sources(k3_k3tchup PUBLIC FILE_SET headers TYPE HEADERS FILES
    include/k3/k3tchup.hpp
    include/k3/k3tchup_main.hpp
    include/k3/k3tchup/assert.hpp
    include/k3/k3tchup/error.hpp
    include/k3/k3tchup/fixture.hpp
    include/k3/k3tchup/hash.hpp
    include/k3/k3tchup/result.hpp
    include/k3/k3tchup/runner.hpp
    include/k3/k3tchup/test.hpp
)

target_include_directories(k3_k3tchup PUBLIC include)

macro(tok3n_discover_tests target assembly_name)

    set(generated_file "${CMAKE_CURRENT_BINARY_DIR}/${target}.k3tchup.list_of_tests.cmake")

    # Must grab it like this because we're inside macro
    get_property(_current_dir TARGET k3_k3tchup PROPERTY SOURCE_DIR)

    add_custom_command(
        TARGET ${target} POST_BUILD
        COMMAND ${target} list ${generated_file}
        COMMAND ${CMAKE_COMMAND}
            -D "ASSEMBLY_NAME=${assembly_name}"
            -D "TEST_TARGET=${target}"
            -D "TEST_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}"
            -D "LIST_OF_TESTS_FILE=${generated_file}"
            -P "${_current_dir}/cmake/discover_tests.cmake"
        BYPRODUCTS ${generated_file}
        VERBATIM
    )

    add_custom_command(
        TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND}
            -D "TEST_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}"
            -D "LIST_OF_TESTS_FILE=${generated_file}"
            -P "${_current_dir}/cmake/include_tests.cmake"
        VERBATIM
    )

endmacro()

add_subdirectory(examples)
