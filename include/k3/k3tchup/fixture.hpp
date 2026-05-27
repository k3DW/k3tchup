// Copyright 2023-2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef K3_K3TCHUP_FIXTURE_HPP
#define K3_K3TCHUP_FIXTURE_HPP

#include <cstddef>
#include <k3/k3tchup/detail/fixture.hpp>
#include <k3/k3tchup/detail/hash.hpp>
#include <k3/k3tchup/runner.hpp>

#define FIXTURE(NAME)                                                         \
    template <std::size_t hash>                                               \
    class _k3_k3tchup_fixture_impl_;                                          \
    template <>                                                               \
    class _k3_k3tchup_fixture_impl_<::k3::k3tchup::detail::simple_hash(NAME)> \
        : private ::k3::k3tchup::detail::fixture                              \
    {                                                                         \
    private:                                                                  \
        static inline bool _k3_k3tchup_init_ = ::k3::k3tchup::runner::get()   \
            .add(::k3::k3tchup::detail::fixture::global<                      \
                ::k3::k3tchup::detail::simple_hash(NAME)>(NAME));             \
    }

#endif // K3_K3TCHUP_FIXTURE_HPP
