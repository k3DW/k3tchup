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
#include <k3/k3tchup/detail/state.hpp>
#include <k3/k3tchup/detail/stateful_tmp.hpp>
#include <k3/k3tchup/detail/void_assigner.hpp>
#include <k3/k3tchup/state.hpp>

#define K3_K3TCHUP_CAT_IMPL_(A, B) A ## B
#define K3_K3TCHUP_CAT_(A, B) K3_K3TCHUP_CAT_IMPL_(A, B)

#define K3_K3TCHUP_EVAL_BOOL_IMPL_(NAME, BOOL) \
    K3_K3TCHUP_ ## NAME ## _ ## BOOL ## _
#define K3_K3TCHUP_EVAL_BOOL_(NAME, BOOL) \
    K3_K3TCHUP_EVAL_BOOL_IMPL_(NAME, BOOL)

#define K3_K3TCHUP_IS_EMPTY_IMPL_(VALUE, ...) VALUE
#define K3_K3TCHUP_IS_EMPTY_(...) \
    K3_K3TCHUP_IS_EMPTY_IMPL_(__VA_OPT__(0,) 1)

#define K3_K3TCHUP_SELECT_IMPL_0_(TRUE, FALSE) FALSE
#define K3_K3TCHUP_SELECT_IMPL_1_(TRUE, FALSE) TRUE
#define K3_K3TCHUP_SELECT_(BOOL, TRUE, FALSE) \
    K3_K3TCHUP_EVAL_BOOL_(SELECT_IMPL, BOOL)(TRUE, FALSE)



// https://developercommunity.visualstudio.com/t/Constexpr-skipping-evaluation-of-switch/11094500
#ifdef _MSC_VER
#define K3_K3TCHUP_CONSTEXPR_ELSE_BLOCKER_
#else
#define K3_K3TCHUP_CONSTEXPR_ELSE_BLOCKER_ switch (0) case 0: default:
#endif



#define K3_K3TCHUP_EVAL_CONDITION_0_(CONDITION) (true)
#define K3_K3TCHUP_EVAL_CONDITION_1_(CONDITION) (CONDITION)

#define K3_K3TCHUP_FATALITY_0_() ::k3::k3tchup::detail::error_fatality::non_fatal
#define K3_K3TCHUP_FATALITY_1_() ::k3::k3tchup::detail::error_fatality::fatal

#define K3_K3TCHUP_FATALITY_RETURN_0_()
#define K3_K3TCHUP_FATALITY_RETURN_1_() return



namespace k3::k3tchup::detail {

template <error_fatality fatality, class F, class T>
consteval bool check_compile_time_error(F, T) {
    if constexpr (has_error<T>) {
        return true;
    } else {
        if constexpr (fatality == error_fatality::non_fatal) {
            return F{}();
        } else if constexpr (fatality == error_fatality::fatal) {
            if constexpr (F{}()) {
                return true;
            } else {
                static_assert(sizeof(set_error_for<T>));
                return false;
            }
        }
    }
}

} // namespace k3::k3tchup::detail



#define K3_K3TCHUP_GENERIC_CHECK_IMPL_(CONDITION, MAKE_CT, MAKE_RT, FATALITY, FATALITY_RETURN, UNIQUE_ID) \
    switch(0) case 0: default:                                                                             \
    if (                                                                                                   \
        const auto UNIQUE_ID = ::k3::k3tchup::detail::context::check(                                      \
            MAKE_CT(::k3::k3tchup::detail::check_compile_time_error<FATALITY()>(                           \
                []() consteval { return static_cast<bool>(CONDITION); },                                   \
                std::integral_constant<std::size_t, ::k3::k3tchup::detail::function_name_hash(             \
                    std::source_location::current().function_name())>{})                                   \
            ),                                                                                             \
            MAKE_RT(static_cast<bool>(CONDITION))                                                          \
        );                                                                                                 \
        UNIQUE_ID                                                                                          \
    ) {}                                                                                                   \
    else                                                                                                   \
        FATALITY_RETURN() ::k3::k3tchup::detail::void_assigner{} =                                         \
            ::k3::k3tchup::detail::context::add_error(UNIQUE_ID, FATALITY())

#if defined(__clang__) && __clang_major__ >= 22
#define K3_K3TCHUP_UNIQUE_IDENTIFIER_(PREFIX) K3_K3TCHUP_CAT_(PREFIX, __LINE__)
#else
#define K3_K3TCHUP_UNIQUE_IDENTIFIER_(PREFIX) K3_K3TCHUP_CAT_(PREFIX, __COUNTER__)
#endif

#define K3_K3TCHUP_GENERIC_CHECK_(CONDITION, IS_CT, IS_RT, IS_FATAL) \
    K3_K3TCHUP_GENERIC_CHECK_IMPL_(CONDITION,                        \
        K3_K3TCHUP_EVAL_BOOL_(EVAL_CONDITION, IS_CT),                \
        K3_K3TCHUP_EVAL_BOOL_(EVAL_CONDITION, IS_RT),                \
        K3_K3TCHUP_EVAL_BOOL_(FATALITY, IS_FATAL),                   \
        K3_K3TCHUP_EVAL_BOOL_(FATALITY_RETURN, IS_FATAL),            \
        K3_K3TCHUP_UNIQUE_IDENTIFIER_(_k3_k3tchup_)                  \
    )



#define K3_K3TCHUP_GENERIC_STATEFUL_CHECK_IMPL_(STATE, CONDITION, FATALITY, FATALITY_RETURN) \
    K3_K3TCHUP_CONSTEXPR_ELSE_BLOCKER_                                                       \
    if (                                                                                     \
        ::k3::k3tchup::detail::state_accessor::check(                                        \
            STATE, FATALITY(), static_cast<bool>(CONDITION))                                 \
    ) {}                                                                                     \
    else                                                                                     \
        FATALITY_RETURN() ::k3::k3tchup::detail::void_assigner{} =                           \
            ::k3::k3tchup::detail::state_accessor::make_message_context(STATE)

#define K3_K3TCHUP_GENERIC_STATEFUL_CHECK_(STATE, CONDITION, IS_FATAL) \
    K3_K3TCHUP_GENERIC_STATEFUL_CHECK_IMPL_(STATE, CONDITION,          \
        K3_K3TCHUP_EVAL_BOOL_(FATALITY, IS_FATAL),                     \
        K3_K3TCHUP_EVAL_BOOL_(FATALITY_RETURN, IS_FATAL)               \
    )



#if defined(__clang__)
// Workaround for the Clang bug where you can't call a
// generic lambda from within a template at compile-time.
// https://github.com/llvm/llvm-project/issues/35052
// https://github.com/llvm/llvm-project/issues/205218

namespace k3::k3tchup::detail::clang_workaround {

template <auto Value>
using constant = std::integral_constant<std::remove_cvref_t<decltype(Value)>, Value>;

struct converter {
    template <class T>
    constexpr operator T() const {
        return T{};
    }
};

template <class F>
constexpr void invoke_packet_nullary(F f) {
    if constexpr (std::invocable<F&&>) {
        std::move(f)();
    }
}

template <class F>
constexpr void invoke_packet_stateful(F f, state& s) {
    if constexpr (std::invocable<F&&, state&>) {
        std::move(f)(s);
    }
}

template <class F>
requires (not std::invocable<F&&, state&>)
constexpr int get_packet_sizes(F&&) {
    return 0;
}

template <class F>
requires std::invocable<F&&, state&>
consteval flat_state_sizes get_packet_sizes(F&& f) {
    auto s = state_accessor::make();
    std::forward<F>(f)(s);
    return state_accessor::sizes(s);
}

template <int dummy, class F>
requires (not std::invocable<F&&, state&>)
constexpr flat_state<flat_state_sizes{}> packet_serialize(constant<dummy>, F&&) {
    return state_accessor::serialize<flat_state_sizes{}>(state_accessor::make());
}

template <flat_state_sizes sizes, class F>
requires std::invocable<F&&, state&>
consteval flat_state<sizes> packet_serialize(constant<sizes>, F&& f) {
    auto s = state_accessor::make();
    std::forward<F>(f)(s);
    return state_accessor::serialize<sizes>(s);
}

} // namespace k3::k3tchup::detail::clang_workaround

#define K3_K3TCHUP_EXEC_PACKET_STATEFUL_(PACKET)                                                    \
    constexpr auto _k3_k3tchup_sizes_ =                                                             \
        __builtin_constant_p(::k3::k3tchup::detail::clang_workaround::get_packet_sizes(PACKET))     \
        ? ::k3::k3tchup::detail::clang_workaround::get_packet_sizes(PACKET)                         \
        : ::k3::k3tchup::detail::clang_workaround::converter{};                                     \
    const ::k3::k3tchup::state _k3_k3tchup_ct_state_ =                                              \
        ::k3::k3tchup::detail::state_accessor::deserialize(                                         \
            ::k3::k3tchup::detail::clang_workaround::packet_serialize(                              \
                ::k3::k3tchup::detail::clang_workaround::constant<_k3_k3tchup_sizes_>{}, PACKET)    \
        );                                                                                          \
    ::k3::k3tchup::state _k3_k3tchup_rt_state_ = ::k3::k3tchup::detail::state_accessor::make();     \
    ::k3::k3tchup::detail::clang_workaround::invoke_packet_stateful(PACKET, _k3_k3tchup_rt_state_); \
    ::k3::k3tchup::detail::state_accessor::report(_k3_k3tchup_ct_state_, _k3_k3tchup_rt_state_)

#define K3_K3TCHUP_EXEC_PACKET_(PACKET)                                                         \
    {                                                                                           \
        using _k3_k3tchup_packet_type_ = decltype(PACKET);                                      \
        if constexpr (std::invocable<_k3_k3tchup_packet_type_>) {                               \
            ::k3::k3tchup::detail::clang_workaround::invoke_packet_nullary(PACKET);             \
        } else if constexpr (std::invocable<_k3_k3tchup_packet_type_, ::k3::k3tchup::state&>) { \
            K3_K3TCHUP_EXEC_PACKET_STATEFUL_(PACKET);                                           \
        } else {                                                                                \
            throw;                                                                              \
        }                                                                                       \
    } static_assert(true, "require semicolon")

#else // ^^^ defined(__clang__) / vvv !defined(__clang__)

#define K3_K3TCHUP_EXEC_PACKET_STATEFUL_(PACKET)                                                \
    const ::k3::k3tchup::state _k3_k3tchup_ct_state_ =                                          \
        ::k3::k3tchup::detail::state_accessor::deserialize([&]() consteval {                    \
            constexpr ::k3::k3tchup::detail::flat_state_sizes sizes = [&]() consteval {         \
                auto _k3_k3tchup_state_ = ::k3::k3tchup::detail::state_accessor::make();        \
                (PACKET)(_k3_k3tchup_state_);                                                   \
                return ::k3::k3tchup::detail::state_accessor::sizes(_k3_k3tchup_state_);        \
            }();                                                                                \
            auto _k3_k3tchup_state_ = ::k3::k3tchup::detail::state_accessor::make();            \
            (PACKET)(_k3_k3tchup_state_);                                                       \
            return ::k3::k3tchup::detail::state_accessor::serialize<sizes>(_k3_k3tchup_state_); \
        }());                                                                                   \
    ::k3::k3tchup::state _k3_k3tchup_rt_state_ = ::k3::k3tchup::detail::state_accessor::make(); \
    (PACKET)(_k3_k3tchup_rt_state_);                                                            \
    ::k3::k3tchup::detail::state_accessor::report(_k3_k3tchup_ct_state_, _k3_k3tchup_rt_state_)

#define K3_K3TCHUP_EXEC_PACKET_(PACKET)                                                         \
    [&]<class _k3_k3tchup_packet_type_>(std::type_identity<_k3_k3tchup_packet_type_>) {         \
        if constexpr (std::invocable<_k3_k3tchup_packet_type_>) {                               \
            (PACKET)();                                                                         \
        } else if constexpr (std::invocable<_k3_k3tchup_packet_type_, ::k3::k3tchup::state&>) { \
            K3_K3TCHUP_EXEC_PACKET_STATEFUL_(PACKET);                                           \
        } else {                                                                                \
            static_assert(std::same_as<_k3_k3tchup_packet_type_, void>);                        \
        }                                                                                       \
    }(std::type_identity<decltype(PACKET)>{})

#endif // defined(__clang__)

#endif // K3_K3TCHUP_MACROS_HPP
