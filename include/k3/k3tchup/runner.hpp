// Copyright 2023-2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef K3_K3TCHUP_RUNNER_HPP
#define K3_K3TCHUP_RUNNER_HPP

#include <map>
#include <string_view>

namespace k3::k3tchup {

namespace detail {
    class test;
    class fixture;
} // namespace detail

class runner
{
public:
    static runner& get();

    int exec(int argc, const char* const argv[]);

    bool add(detail::fixture& fixture);
    bool add(std::string_view fixture_name, detail::test&& t);

private:
    runner() = default;

    std::map<std::string_view, detail::fixture*> _fixtures;
};

} // namespace k3::k3tchup

#endif // K3_K3TCHUP_RUNNER_HPP
