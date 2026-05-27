// Copyright 2024-2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef K3_K3TCHUP_DETAIL_HASH_HPP
#define K3_K3TCHUP_DETAIL_HASH_HPP

#include <bit>
#include <string_view>

namespace k3::k3tchup::detail {

constexpr std::size_t simple_hash(std::string_view s) {
    std::size_t hash = 0;
    for (char c : s) {
        hash = std::rotl(hash, 1) + static_cast<std::size_t>(c);
    }
    return hash;
}

constexpr std::size_t simple_hash(std::string_view s1, std::string_view s2) {
    return std::rotl(simple_hash(s1), 1) + simple_hash(s2);
}

constexpr std::size_t function_name_hash(std::string_view s) {
#ifdef _MSC_VER
    // https://developercommunity.visualstudio.com/t/__builtin_FUNCSIG-gives-different-resu/11069467
    constexpr std::string_view to_find = " __cdecl ";
    const std::size_t pos = s.find(to_find);
    if (pos != std::string_view::npos) {
        s.remove_prefix(pos + to_find.size() + 1);
    }
#endif
    return simple_hash(s);
}

} // namespace k3::k3tchup::detail

#endif // K3_K3TCHUP_DETAIL_HASH_HPP
