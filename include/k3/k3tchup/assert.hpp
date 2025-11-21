// Copyright 2023-2025 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef K3_K3TCHUP_ASSERT_HPP
#define K3_K3TCHUP_ASSERT_HPP

#define K3_K3TCHUP_EVAL_BOOL_IMPL_(NAME, BOOL) \
    K3_K3TCHUP_ ## NAME ## _ ## BOOL ## _
#define K3_K3TCHUP_EVAL_BOOL_(NAME, BOOL) \
    K3_K3TCHUP_EVAL_BOOL_IMPL_(NAME, BOOL)

#define K3_K3TCHUP_IS_EMPTY_IMPL_(VALUE, ...) VALUE
#define K3_K3TCHUP_IS_EMPTY_(...) \
    K3_K3TCHUP_IS_EMPTY_IMPL_(__VA_OPT__(0,) 1)

namespace k3::k3tchup::detail {

consteval bool eval_condition(bool b) {
    return b;
}

} // namespace k3::k3tchup::detail

#define K3_K3TCHUP_EVAL_CONDITION_0_(CONDITION) (true)
#define K3_K3TCHUP_EVAL_CONDITION_1_(CONDITION) (CONDITION)

#define K3_K3TCHUP_ERROR_FATALITY_0_(RESULT) \
    ::k3::k3tchup::void_assignment_helper{} = ::k3::k3tchup::context::add_error(RESULT, ::k3::k3tchup::error_fatality::non_fatal)
#define K3_K3TCHUP_ERROR_FATALITY_1_(RESULT) \
    return ::k3::k3tchup::void_assignment_helper{} = ::k3::k3tchup::context::add_error(RESULT, ::k3::k3tchup::error_fatality::fatal)

#define K3_K3TCHUP_DEPENDENT_CONDITION_0_(CONDITION, ARG) \
    if constexpr (CONDITION) { ARG; }
#define K3_K3TCHUP_DEPENDENT_CONDITION_1_(...) \
    (void)0;



#define K3_K3TCHUP_GENERIC_CHECK_IMPL_(CONDITION, MAKE_CT, MAKE_RT, MAKE_ERROR, ...) \
    switch(0) case 0: default:                                                       \
    if (                                                                             \
        const auto _k3tchup_res_ = ::k3::k3tchup::context::check(                    \
            ::k3::k3tchup::detail::eval_condition(MAKE_CT(bool{(CONDITION)})),       \
            MAKE_RT(bool{(CONDITION)})                                               \
        );                                                                           \
        _k3tchup_res_                                                                \
    ) {                                                                              \
        K3_K3TCHUP_EVAL_BOOL_(DEPENDENT_CONDITION,                                   \
            K3_K3TCHUP_IS_EMPTY_(__VA_ARGS__))(CONDITION, __VA_ARGS__)               \
    }                                                                                \
    else                                                                             \
        MAKE_ERROR(_k3tchup_res_)

#define K3_K3TCHUP_GENERIC_CHECK_(CONDITION, IS_CT, IS_RT, IS_FATAL, ...) \
    K3_K3TCHUP_GENERIC_CHECK_IMPL_(CONDITION,                             \
        K3_K3TCHUP_EVAL_BOOL_(EVAL_CONDITION, IS_CT),                     \
        K3_K3TCHUP_EVAL_BOOL_(EVAL_CONDITION, IS_RT),                     \
        K3_K3TCHUP_EVAL_BOOL_(ERROR_FATALITY, IS_FATAL),                  \
        __VA_ARGS__                                                       \
    )

#define ASSERT_COMPILE_TIME(CONDITION, ...)         K3_K3TCHUP_GENERIC_CHECK_(CONDITION, 1, 0, 1, __VA_ARGS__)
#define ASSERT_RUN_TIME(CONDITION, ...)             K3_K3TCHUP_GENERIC_CHECK_(CONDITION, 0, 1, 1, __VA_ARGS__)
#define ASSERT_COMPILE_AND_RUN_TIME(CONDITION, ...) K3_K3TCHUP_GENERIC_CHECK_(CONDITION, 1, 1, 1, __VA_ARGS__)

#define EXPECT_COMPILE_TIME(CONDITION, ...)         K3_K3TCHUP_GENERIC_CHECK_(CONDITION, 1, 0, 0, __VA_ARGS__)
#define EXPECT_RUN_TIME(CONDITION, ...)             K3_K3TCHUP_GENERIC_CHECK_(CONDITION, 0, 1, 0, __VA_ARGS__)
#define EXPECT_COMPILE_AND_RUN_TIME(CONDITION, ...) K3_K3TCHUP_GENERIC_CHECK_(CONDITION, 1, 1, 0, __VA_ARGS__)



// ASSERT_THAT does not ignore its internal non-fatal errors,
// and treats them as fatal errors.
#define ASSERT_THAT(FRAGMENT)                                                          \
    {                                                                                  \
        ::k3::k3tchup::context::trace_context _k3tchup_ctx_;                           \
        const std::size_t _k3tchup_starting_ = ::k3::k3tchup::context::total_errors(); \
        (FRAGMENT)();                                                                  \
        const std::size_t _k3tchup_ending_ = ::k3::k3tchup::context::total_errors();   \
        if (_k3tchup_ending_ != _k3tchup_starting_)                                    \
            return;                                                                    \
    } static_assert(true, "require semicolon")

// EXPECT_THAT ignores all the internal errors.
#define EXPECT_THAT(FRAGMENT)                                \
    {                                                        \
        ::k3::k3tchup::context::trace_context _k3tchup_ctx_; \
        (FRAGMENT)();                                        \
    } static_assert(true, "require semicolon")

#endif // K3_K3TCHUP_ASSERT_HPP
