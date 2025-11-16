macro(k3_k3tchup_discover_tests target assembly_name)

    set(generated_file "${CMAKE_CURRENT_BINARY_DIR}/${target}.k3tchup.list_of_tests.cmake")

    # Must grab it like this because we're inside macro
    get_property(_k3tchup_dir TARGET k3_k3tchup PROPERTY SOURCE_DIR)

    add_custom_command(
        TARGET ${target} POST_BUILD
        COMMAND ${target} list ${generated_file}
        COMMAND ${CMAKE_COMMAND}
            -D "ASSEMBLY_NAME=${assembly_name}"
            -D "TEST_TARGET=${target}"
            -D "TEST_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}"
            -D "LIST_OF_TESTS_FILE=${generated_file}"
            -P "${_k3tchup_dir}/cmake/discover_tests_script.cmake"
        BYPRODUCTS ${generated_file}
        VERBATIM
    )

    add_custom_command(
        TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND}
            -D "TEST_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}"
            -D "LIST_OF_TESTS_FILE=${generated_file}"
            -P "${_k3tchup_dir}/cmake/include_tests_script.cmake"
        VERBATIM
    )

endmacro()
