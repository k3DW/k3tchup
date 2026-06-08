// Copyright 2023-2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef K3_K3TCHUP_MACROS_HPP
#define K3_K3TCHUP_MACROS_HPP

#include <source_location>
#include <type_traits>
#include <k3/k3tchup/detail/context.hpp>
#include <k3/k3tchup/detail/error.hpp>
#include <k3/k3tchup/detail/hash.hpp>
#include <k3/k3tchup/detail/stateful_tmp.hpp>
#include <k3/k3tchup/detail/void_assigner.hpp>

#define K3_K3TCHUP_CAT_IMPL_(A, B) A ## B
#define K3_K3TCHUP_CAT_(A, B) K3_K3TCHUP_CAT_IMPL_(A, B)

#define K3_K3TCHUP_EVAL_BOOL_IMPL_(NAME, BOOL) \
    K3_K3TCHUP_ ## NAME ## _ ## BOOL ## _
#define K3_K3TCHUP_EVAL_BOOL_(NAME, BOOL) \
    K3_K3TCHUP_EVAL_BOOL_IMPL_(NAME, BOOL)

#define K3_K3TCHUP_IS_EMPTY_IMPL_(VALUE, ...) VALUE
#define K3_K3TCHUP_IS_EMPTY_(...) \
    K3_K3TCHUP_IS_EMPTY_IMPL_(__VA_OPT__(0,) 1)



// https://developercommunity.visualstudio.com/t/Constexpr-skipping-evaluation-of-switch/11094500
#ifdef _MSC_VER
#define K3_K3TCHUP_CONSTEXPR_ELSE_BLOCKER_
#else
#define K3_K3TCHUP_CONSTEXPR_ELSE_BLOCKER_ switch (0) case 0: default:
#endif



#define K3_K3TCHUP_EVAL_CONDITION_0_(CONDITION) (true)
#define K3_K3TCHUP_EVAL_CONDITION_1_(CONDITION) (CONDITION)

#define K3_K3TCHUP_ERROR_FATALITY_0_(RESULT) \
    ::k3::k3tchup::detail::void_assigner{} = ::k3::k3tchup::detail::context::add_error(RESULT, ::k3::k3tchup::detail::error_fatality::non_fatal)
#define K3_K3TCHUP_ERROR_FATALITY_1_(RESULT) \
    return ::k3::k3tchup::detail::void_assigner{} = ::k3::k3tchup::detail::context::add_error(RESULT, ::k3::k3tchup::detail::error_fatality::fatal)

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



#define K3_K3TCHUP_GENERIC_CHECK_IMPL_(CONDITION, MAKE_CT, MAKE_RT, MAKE_ERROR, CHECK_CT_ERROR, UNIQUE_ID)     \
    switch(0) case 0: default:                                                                                 \
    if (                                                                                                       \
        const auto UNIQUE_ID = ::k3::k3tchup::detail::context::check(                                          \
            [&]<class _k3_k3tchup_type_parameter_>(_k3_k3tchup_type_parameter_) consteval {                    \
                if constexpr (::k3::k3tchup::detail::has_error<_k3_k3tchup_type_parameter_>) {                 \
                    return true;                                                                               \
                } else {                                                                                       \
                    CHECK_CT_ERROR(MAKE_CT(bool{(CONDITION)}), _k3_k3tchup_type_parameter_);                   \
                }                                                                                              \
            }(std::integral_constant<std::size_t,                                                              \
                ::k3::k3tchup::detail::function_name_hash(std::source_location::current().function_name())>{}) \
            ,                                                                                                  \
            MAKE_RT(bool{(CONDITION)})                                                                         \
        );                                                                                                     \
        UNIQUE_ID                                                                                              \
    ) {}                                                                                                       \
    else                                                                                                       \
        MAKE_ERROR(UNIQUE_ID)

#if defined(__clang__) && __clang_major__ >= 22
#define K3_K3TCHUP_UNIQUE_IDENTIFIER_(PREFIX) K3_K3TCHUP_CAT_(PREFIX, __LINE__)
#else
#define K3_K3TCHUP_UNIQUE_IDENTIFIER_(PREFIX) K3_K3TCHUP_CAT_(PREFIX, __COUNTER__)
#endif

#define K3_K3TCHUP_GENERIC_CHECK_(CONDITION, IS_CT, IS_RT, IS_FATAL) \
    K3_K3TCHUP_GENERIC_CHECK_IMPL_(CONDITION,                        \
        K3_K3TCHUP_EVAL_BOOL_(EVAL_CONDITION, IS_CT),                \
        K3_K3TCHUP_EVAL_BOOL_(EVAL_CONDITION, IS_RT),                \
        K3_K3TCHUP_EVAL_BOOL_(ERROR_FATALITY, IS_FATAL),             \
        K3_K3TCHUP_EVAL_BOOL_(CHECK_CT_ERROR, IS_FATAL),             \
        K3_K3TCHUP_UNIQUE_IDENTIFIER_(_k3_k3tchup_)                  \
    )

#endif // K3_K3TCHUP_MACROS_HPP
