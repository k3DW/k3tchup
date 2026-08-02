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

#define ASSERT_STATEFUL(STATE, CONDITION)      K3_K3TCHUP_GENERIC_STATEFUL_CHECK_(STATE, CONDITION, 1)
#define EXPECT_STATEFUL(STATE, CONDITION)      K3_K3TCHUP_GENERIC_STATEFUL_CHECK_(STATE, CONDITION, 0)

#define ASSERT(ARG0, ...)                                                                               \
    K3_K3TCHUP_SELECT_(K3_K3TCHUP_IS_EMPTY_(__VA_ARGS__), ASSERT_COMPILE_AND_RUN_TIME, ASSERT_STATEFUL) \
    (ARG0 __VA_OPT__(,) __VA_ARGS__)
#define EXPECT(ARG0, ...)                                                                               \
    K3_K3TCHUP_SELECT_(K3_K3TCHUP_IS_EMPTY_(__VA_ARGS__), EXPECT_COMPILE_AND_RUN_TIME, EXPECT_STATEFUL) \
    (ARG0 __VA_OPT__(,) __VA_ARGS__)



namespace k3::k3tchup::detail {

template <class F>
void exec_packet(F packet) {
    if constexpr (std::invocable<F>) {
        (packet)();
    } else if constexpr (std::invocable<F, state&>) {
        K3_K3TCHUP_EXEC_PACKET_STATEFUL_(packet);
    } else {
        static_assert(std::same_as<F, void>);
    }
}

} // namespace k3::k3tchup::detail

// ASSERT_THAT does not ignore its internal non-fatal errors,
// and treats them as fatal errors.
#define ASSERT_THAT(PACKET)                                                                       \
    {                                                                                             \
        ::k3::k3tchup::detail::trace_context _k3_k3tchup_ctx_;                                    \
        const std::size_t _k3_k3tchup_starting_ = ::k3::k3tchup::detail::context::total_errors(); \
        ::k3::k3tchup::detail::exec_packet(PACKET);                                               \
        const std::size_t _k3_k3tchup_ending_ = ::k3::k3tchup::detail::context::total_errors();   \
        if (_k3_k3tchup_ending_ != _k3_k3tchup_starting_)                                         \
            return;                                                                               \
    } static_assert(true, "require semicolon")

// EXPECT_THAT ignores all the internal errors.
#define EXPECT_THAT(PACKET)                                    \
    {                                                          \
        ::k3::k3tchup::detail::trace_context _k3_k3tchup_ctx_; \
        ::k3::k3tchup::detail::exec_packet(PACKET);            \
    } static_assert(true, "require semicolon")

#endif // K3_K3TCHUP_ASSERT_HPP
