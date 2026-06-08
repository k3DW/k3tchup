// Copyright 2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef K3_K3TCHUP_DETAIL_STATE_HPP
#define K3_K3TCHUP_DETAIL_STATE_HPP

#include <array>
#include <cstddef>
#include <source_location>
#include <string_view>
#include <string>
#include <type_traits>
#include <k3/k3tchup/detail/error.hpp>
#include <k3/k3tchup/state.hpp>

namespace k3::k3tchup::detail {

struct state_error_entry {
    std::size_t index;
    error_fatality fatality;
    std::size_t num_chars;
    std::source_location location;
};

struct flat_state_sizes {
    std::size_t count;
    std::size_t total_chars;
};

template <flat_state_sizes sizes>
class flat_state {
    friend struct state_accessor;
    constexpr flat_state() = default;
    std::size_t num_checks = 0;
    std::array<state_error_entry, sizes.count> entries{};
    std::array<char, sizes.total_chars> messages{};
};

struct state_accessor {
    static constexpr state make() {
        return {};
    }

    static constexpr bool check(state& s, error_fatality f, bool b, std::source_location location = std::source_location::current()) {
        if (!b) {
            s.errors.value().push_back({
                .index = s.num_checks,
                .fatality = f,
                .location = location,
            });
        }
        ++s.num_checks;
        return b;
    }

    struct message_context {
        std::string* str;
        constexpr message_context operator<<(std::string_view sv) const {
            str->append(sv);
            return *this;
        }
    };

    static constexpr message_context make_message_context(state& s) {
        return message_context{ &s.errors.value().back().message };
    }

    static constexpr flat_state_sizes sizes(const state& s) {
        std::size_t total_chars = 0;
        for (const auto& e : s.errors.value()) {
            total_chars += e.message.size();
        }
        return {
            .count = s.errors.value().size(),
            .total_chars = total_chars
        };
    }

    template <flat_state_sizes sizes>
    static constexpr flat_state<sizes> serialize(const state& s) {
        flat_state<sizes> errors{};
        errors.num_checks = s.num_checks;
        auto entry_it = errors.entries.begin();
        auto message_it = errors.messages.begin();
        for (const auto& e : s.errors.value()) {
            *entry_it++ = {
                .index = e.index,
                .fatality = e.fatality,
                .num_chars = e.message.size(),
                .location = e.location,
            };
            for (char c : e.message) {
                *message_it++ = c;
            }
        }
        if (entry_it != errors.entries.end()) {
            throw; // logic error
        }
        if (message_it != errors.messages.end()) {
            throw; // logic error
        }
        return errors;
    }

    template <flat_state_sizes sizes>
    static state deserialize(const flat_state<sizes>& errors) {
        state s{};
        s.num_checks = errors.num_checks;
        auto& vec = s.errors.emplace();
        auto it = errors.messages.begin();
        for (const auto& entry : errors.entries) {
            vec.push_back({
                .index = entry.index,
                .fatality = entry.fatality,
                .message{it, it + entry.num_chars},
                .location = entry.location,
            });
            it += entry.num_chars;
        }
        if (it != errors.messages.end()) {
            throw; // logic error
        }
        return s;
    }

    static void report(const state& ct, const state& rt);
};

} // namespace k3::k3tchup::detail

#endif // K3_K3TCHUP_DETAIL_STATE_HPP
