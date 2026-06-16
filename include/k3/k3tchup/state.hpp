// Copyright 2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef K3_K3TCHUP_STATE_HPP
#define K3_K3TCHUP_STATE_HPP

#include <cstddef>
#include <optional>
#include <source_location>
#include <string_view>
#include <string>
#include <utility>
#include <vector>
#include <k3/k3tchup/detail/error.hpp>

namespace k3::k3tchup {

namespace detail {

struct state_accessor;

struct state_error {
    std::size_t index;
    error_fatality fatality;
    std::string message{};
    std::source_location location;
};

bool operator==(const state_error& lhs, const state_error& rhs);

} // namespace detail

class state {
    friend struct detail::state_accessor;
    constexpr state() = default;
    constexpr state(const state&) = default;
    constexpr state(state&&) = default;
    constexpr state& operator=(const state&) = default;
    constexpr state& operator=(state&&) = default;
    std::size_t num_checks = 0;
    std::optional<std::vector<detail::state_error>> errors{ std::in_place };
};

} // namespace k3::k3tchup

#endif // K3_K3TCHUP_STATE_HPP
