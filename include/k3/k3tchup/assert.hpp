// Copyright 2023-2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef K3_K3TCHUP_ASSERT_HPP
#define K3_K3TCHUP_ASSERT_HPP

#include <cstddef>
#include <k3/k3tchup/detail/context.hpp>
#include <k3/k3tchup/detail/macros.hpp>

#define ASSERT_COMPILE_TIME(CONDITION)         K3_K3TCHUP_GENERIC_CHECK_(CONDITION, 1, 0, 1)
#define ASSERT_RUN_TIME(CONDITION)             K3_K3TCHUP_GENERIC_CHECK_(CONDITION, 0, 1, 1)
#define ASSERT_COMPILE_AND_RUN_TIME(CONDITION) K3_K3TCHUP_GENERIC_CHECK_(CONDITION, 1, 1, 1)

#define EXPECT_COMPILE_TIME(CONDITION)         K3_K3TCHUP_GENERIC_CHECK_(CONDITION, 1, 0, 0)
#define EXPECT_RUN_TIME(CONDITION)             K3_K3TCHUP_GENERIC_CHECK_(CONDITION, 0, 1, 0)
#define EXPECT_COMPILE_AND_RUN_TIME(CONDITION) K3_K3TCHUP_GENERIC_CHECK_(CONDITION, 1, 1, 0)

// ASSERT_THAT does not ignore its internal non-fatal errors,
// and treats them as fatal errors.
#define ASSERT_THAT(PACKET)                                                                    \
    {                                                                                          \
        ::k3::k3tchup::detail::trace_context _k3tchup_ctx_;                                    \
        const std::size_t _k3tchup_starting_ = ::k3::k3tchup::detail::context::total_errors(); \
        (PACKET)();                                                                            \
        const std::size_t _k3tchup_ending_ = ::k3::k3tchup::detail::context::total_errors();   \
        if (_k3tchup_ending_ != _k3tchup_starting_)                                            \
            return;                                                                            \
    } static_assert(true, "require semicolon")

// EXPECT_THAT ignores all the internal errors.
#define EXPECT_THAT(PACKET)                                  \
    {                                                        \
        ::k3::k3tchup::detail::trace_context _k3tchup_ctx_;  \
        (PACKET)();                                          \
    } static_assert(true, "require semicolon")

#endif // K3_K3TCHUP_ASSERT_HPP
