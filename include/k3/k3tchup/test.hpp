// Copyright 2023-2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef K3_K3TCHUP_TEST_HPP
#define K3_K3TCHUP_TEST_HPP

#include <cstddef>
#include <type_traits>
#include <k3/k3tchup/detail/fixture.hpp>
#include <k3/k3tchup/detail/hash.hpp>
#include <k3/k3tchup/detail/macros.hpp>
#include <k3/k3tchup/detail/test.hpp>
#include <k3/k3tchup/runner.hpp>
#include <k3/k3tchup/state.hpp>

#define TEST(FIXTURE_NAME, NAME)                                                                            \
    template <std::size_t hash>                                                                             \
    class _k3_k3tchup_test_impl_;                                                                           \
    template <std::size_t hash>                                                                             \
    class _k3_k3tchup_fixture_impl_;                                                                        \
    template <>                                                                                             \
    class _k3_k3tchup_test_impl_<::k3::k3tchup::detail::simple_hash(FIXTURE_NAME, NAME)>                    \
    {                                                                                                       \
    private:                                                                                                \
        static_assert(                                                                                      \
            std::is_base_of_v<::k3::k3tchup::detail::fixture,                                               \
                _k3_k3tchup_fixture_impl_<::k3::k3tchup::detail::simple_hash(FIXTURE_NAME)>>,               \
            "Fixture \"" FIXTURE_NAME "\" has not been declared in this namespace.");                       \
        static void _k3_k3tchup_run_();                                                                     \
        static inline const bool _k3_k3tchup_init_ = ::k3::k3tchup::runner::get()                           \
            .add(FIXTURE_NAME, ::k3::k3tchup::detail::test(NAME, &_k3_k3tchup_run_));                       \
    };                                                                                                      \
    void _k3_k3tchup_test_impl_<::k3::k3tchup::detail::simple_hash(FIXTURE_NAME, NAME)>::_k3_k3tchup_run_()

namespace k3::k3tchup::detail {

// We need this extra level of function indirection to make things work in constexpr
template <void(*fn)(k3::k3tchup::state&)>
void exec_stateful() {
    K3_K3TCHUP_EXEC_PACKET_STATEFUL_(fn);
}

} // namespace k3::k3tchup::detail

#define TEST_CONSTEXPR(FIXTURE_NAME, NAME, STATE_IDENTIFIER)                                       \
    template <std::size_t hash>                                                                    \
    class _k3_k3tchup_test_impl_;                                                                  \
    template <std::size_t hash>                                                                    \
    class _k3_k3tchup_fixture_impl_;                                                               \
    template <>                                                                                    \
    class _k3_k3tchup_test_impl_<::k3::k3tchup::detail::simple_hash(FIXTURE_NAME, NAME)>           \
    {                                                                                              \
    private:                                                                                       \
        static_assert(                                                                             \
            std::is_base_of_v<::k3::k3tchup::detail::fixture,                                      \
                _k3_k3tchup_fixture_impl_<::k3::k3tchup::detail::simple_hash(FIXTURE_NAME)>>,      \
            "Fixture \"" FIXTURE_NAME "\" has not been declared in this namespace.");              \
        static constexpr void _k3_k3tchup_run_(::k3::k3tchup::state&);                             \
        static inline const bool _k3_k3tchup_init_ = ::k3::k3tchup::runner::get()                  \
            .add(FIXTURE_NAME, ::k3::k3tchup::detail::test(                                        \
                NAME, +[]{ ::k3::k3tchup::detail::exec_stateful<_k3_k3tchup_run_>(); }));          \
    };                                                                                             \
    constexpr void _k3_k3tchup_test_impl_<::k3::k3tchup::detail::simple_hash(FIXTURE_NAME, NAME)>  \
        ::_k3_k3tchup_run_([[maybe_unused]] ::k3::k3tchup::state& STATE_IDENTIFIER)

#endif // K3_K3TCHUP_TEST_HPP
