# Copyright 2025 Braden Ganetsky
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

add_executable(example_vector_of_optionals vector_of_optionals.cpp)
target_link_libraries(example_vector_of_optionals k3::k3tchup)
k3_k3tchup_discover_tests(example_vector_of_optionals example_vector_of_optionals)

add_executable(example_edge_cases edge_cases.cpp)
target_link_libraries(example_edge_cases k3::k3tchup)
k3_k3tchup_discover_tests(example_edge_cases example_edge_cases)
