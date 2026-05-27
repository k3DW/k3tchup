// Copyright 2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef K3_K3TCHUP_DETAIL_STATEFUL_TMP_HPP
#define K3_K3TCHUP_DETAIL_STATEFUL_TMP_HPP

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

#endif // K3_K3TCHUP_DETAIL_STATEFUL_TMP_HPP
