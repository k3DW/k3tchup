// Copyright 2023-2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef K3_K3TCHUP_ASSERT_HPP
#define K3_K3TCHUP_ASSERT_HPP

#include <cstddef>
#include <k3/k3tchup/detail/context.hpp>
#include <k3/k3tchup/detail/error.hpp>
#include <k3/k3tchup/detail/macros.hpp>
#include <k3/k3tchup/detail/state.hpp>

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

#define ASSERT_DEDUCE(S, CONDITION)                                        \
    K3_K3TCHUP_CONSTEXPR_ELSE_BLOCKER_                                     \
    if (                                                                   \
        ::k3::k3tchup::detail::state_accessor::check(S,                    \
            ::k3::k3tchup::detail::error_fatality::fatal,                  \
            static_cast<bool>(CONDITION))                                  \
    ) {}                                                                   \
    else                                                                   \
        return ::k3::k3tchup::detail::void_assigner{} =                    \
            ::k3::k3tchup::detail::state_accessor::make_message_context(S)

#define EXPECT_DEDUCE(S, CONDITION)                                        \
    K3_K3TCHUP_CONSTEXPR_ELSE_BLOCKER_                                     \
    if (                                                                   \
        ::k3::k3tchup::detail::state_accessor::check(S,                    \
            ::k3::k3tchup::detail::error_fatality::non_fatal,              \
            static_cast<bool>(CONDITION))                                  \
    ) {}                                                                   \
    else                                                                   \
        ::k3::k3tchup::detail::void_assigner{} =                           \
            ::k3::k3tchup::detail::state_accessor::make_message_context(S)

#define EXPECT_THAT_2(PACKET)                                                               \
    {                                                                                       \
        ::k3::k3tchup::detail::trace_context _k3tchup_ctx_;                                 \
        const ::k3::k3tchup::state ct_state =                                               \
            ::k3::k3tchup::detail::state_accessor::deserialize([&]() consteval {            \
                constexpr ::k3::k3tchup::detail::flat_state_sizes sizes = [&]() consteval { \
                    auto s = ::k3::k3tchup::detail::state_accessor::make();                 \
                    (PACKET)(s);                                                            \
                    return ::k3::k3tchup::detail::state_accessor::sizes(s);                 \
                }();                                                                        \
                auto s = ::k3::k3tchup::detail::state_accessor::make();                     \
                (PACKET)(s);                                                                \
                return ::k3::k3tchup::detail::state_accessor::serialize<sizes>(s);          \
            }());                                                                           \
        const ::k3::k3tchup::state rt_state = [&]() {                                       \
            auto s = ::k3::k3tchup::detail::state_accessor::make();                         \
            (PACKET)(s);                                                                    \
            return s;                                                                       \
        }();                                                                                \
        ::k3::k3tchup::detail::state_accessor::report(ct_state, rt_state);                  \
    } static_assert(true, "require semicolon")

#endif // K3_K3TCHUP_ASSERT_HPP
