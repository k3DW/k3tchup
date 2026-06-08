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
    friend constexpr bool operator==(const state_error& lhs, const state_error& rhs) {
        return
            lhs.index == rhs.index and
            lhs.fatality == rhs.fatality and
            lhs.message == rhs.message and
            lhs.location.line() == rhs.location.line() and
            lhs.location.column() == rhs.location.column() and
            std::string_view(lhs.location.file_name()) == std::string_view(rhs.location.file_name()) and
            std::string_view(lhs.location.function_name()) == std::string_view(rhs.location.function_name());
    }
};

} // namespace detail

class state {
    friend struct detail::state_accessor;
    constexpr state() = default;
    std::size_t num_checks = 0;
    std::optional<std::vector<detail::state_error>> errors{ std::in_place };
};

} // namespace k3::k3tchup

#endif // K3_K3TCHUP_STATE_HPP
