// Copyright 2023-2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef K3_K3TCHUP_ASSERT_HPP
#define K3_K3TCHUP_ASSERT_HPP

#define K3_K3TCHUP_CAT_IMPL_(A, B) A ## B
#define K3_K3TCHUP_CAT_(A, B) K3_K3TCHUP_CAT_IMPL_(A, B)

#define K3_K3TCHUP_EVAL_BOOL_IMPL_(NAME, BOOL) \
    K3_K3TCHUP_ ## NAME ## _ ## BOOL ## _
#define K3_K3TCHUP_EVAL_BOOL_(NAME, BOOL) \
    K3_K3TCHUP_EVAL_BOOL_IMPL_(NAME, BOOL)

#define K3_K3TCHUP_IS_EMPTY_IMPL_(VALUE, ...) VALUE
#define K3_K3TCHUP_IS_EMPTY_(...) \
    K3_K3TCHUP_IS_EMPTY_IMPL_(__VA_OPT__(0,) 1)

namespace k3::k3tchup::detail {

template <class T>
struct error_key {
    friend auto find(error_key<T>);
};

template <class T>
struct set_error_for {
    friend auto find(error_key<T>) { return 0; }
};

template <class T, class Unique = decltype([]{})>
concept has_error = requires {
    find(error_key<T>{});
    typename Unique;
};

} // namespace k3::k3tchup::detail

#define K3_K3TCHUP_EVAL_CONDITION_0_(CONDITION) (true)
#define K3_K3TCHUP_EVAL_CONDITION_1_(CONDITION) (CONDITION)

#define K3_K3TCHUP_ERROR_FATALITY_0_(RESULT) \
    ::k3::k3tchup::void_assignment_helper{} = ::k3::k3tchup::context::add_error(RESULT, ::k3::k3tchup::error_fatality::non_fatal)
#define K3_K3TCHUP_ERROR_FATALITY_1_(RESULT) \
    return ::k3::k3tchup::void_assignment_helper{} = ::k3::k3tchup::context::add_error(RESULT, ::k3::k3tchup::error_fatality::fatal)

#define K3_K3TCHUP_CHECK_CT_ERROR_0_(CONDITION, ...) \
    return (CONDITION)
#define K3_K3TCHUP_CHECK_CT_ERROR_1_(CONDITION, TYPE)                      \
    if constexpr (CONDITION) {                                             \
        return true;                                                       \
    } else {                                                               \
        static_assert(sizeof(::k3::k3tchup::detail::set_error_for<TYPE>)); \
        return false;                                                      \
    }                                                                      \
    static_assert(true, "require semicolon")

#define K3_K3TCHUP_DEPENDENT_CONDITION_0_(CONDITION, ARG)             \
    ([&]<class T>(T) {                                                \
        if constexpr ((sizeof(T)==sizeof(int)) && CONDITION) { ARG; } \
    }(0));
#define K3_K3TCHUP_DEPENDENT_CONDITION_1_(...) \
    (void)0;



#define K3_K3TCHUP_GENERIC_CHECK_IMPL_(CONDITION, MAKE_CT, MAKE_RT, MAKE_ERROR, CHECK_CT_ERROR, UNIQUE_ID, ...) \
    switch(0) case 0: default:                                                                                  \
    if (                                                                                                        \
        const auto UNIQUE_ID = ::k3::k3tchup::context::check(                                                   \
            [&]<class _k3_k3tchup_type_parameter_>(_k3_k3tchup_type_parameter_) consteval {                     \
                if constexpr (::k3::k3tchup::detail::has_error<_k3_k3tchup_type_parameter_>) {                  \
                    return true;                                                                                \
                } else {                                                                                        \
                    CHECK_CT_ERROR(MAKE_CT(bool{(CONDITION)}), _k3_k3tchup_type_parameter_);                    \
                }                                                                                               \
            }(std::integral_constant<std::size_t,                                                               \
                ::k3::k3tchup::function_name_hash(std::source_location::current().function_name())>{})          \
            ,                                                                                                   \
            MAKE_RT(bool{(CONDITION)})                                                                          \
        );                                                                                                      \
        UNIQUE_ID                                                                                               \
    ) {                                                                                                         \
        K3_K3TCHUP_EVAL_BOOL_(DEPENDENT_CONDITION,                                                              \
            K3_K3TCHUP_IS_EMPTY_(__VA_ARGS__))(CONDITION, __VA_ARGS__)                                          \
    }                                                                                                           \
    else                                                                                                        \
        MAKE_ERROR(UNIQUE_ID)

#if defined(__clang__) && __clang_major__ >= 22
#define K3_K3TCHUP_UNIQUE_IDENTIFIER_(PREFIX) K3_K3TCHUP_CAT_(PREFIX, __LINE__)
#else
#define K3_K3TCHUP_UNIQUE_IDENTIFIER_(PREFIX) K3_K3TCHUP_CAT_(PREFIX, __COUNTER__)
#endif

#define K3_K3TCHUP_GENERIC_CHECK_(CONDITION, IS_CT, IS_RT, IS_FATAL, ...) \
    K3_K3TCHUP_GENERIC_CHECK_IMPL_(CONDITION,                             \
        K3_K3TCHUP_EVAL_BOOL_(EVAL_CONDITION, IS_CT),                     \
        K3_K3TCHUP_EVAL_BOOL_(EVAL_CONDITION, IS_RT),                     \
        K3_K3TCHUP_EVAL_BOOL_(ERROR_FATALITY, IS_FATAL),                  \
        K3_K3TCHUP_EVAL_BOOL_(CHECK_CT_ERROR, IS_FATAL),                  \
        K3_K3TCHUP_UNIQUE_IDENTIFIER_(_k3_k3tchup_),                      \
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
#define ASSERT_THAT(PACKET)                                                            \
    {                                                                                  \
        ::k3::k3tchup::context::trace_context _k3tchup_ctx_;                           \
        const std::size_t _k3tchup_starting_ = ::k3::k3tchup::context::total_errors(); \
        (PACKET)();                                                                    \
        const std::size_t _k3tchup_ending_ = ::k3::k3tchup::context::total_errors();   \
        if (_k3tchup_ending_ != _k3tchup_starting_)                                    \
            return;                                                                    \
    } static_assert(true, "require semicolon")

// EXPECT_THAT ignores all the internal errors.
#define EXPECT_THAT(PACKET)                                  \
    {                                                        \
        ::k3::k3tchup::context::trace_context _k3tchup_ctx_; \
        (PACKET)();                                          \
    } static_assert(true, "require semicolon")

#endif // K3_K3TCHUP_ASSERT_HPP
