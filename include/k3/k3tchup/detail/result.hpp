// Copyright 2023-2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef K3_K3TCHUP_DETAIL_RESULT_HPP
#define K3_K3TCHUP_DETAIL_RESULT_HPP

#include <cstddef>
#include <iosfwd>
#include <ostream>
#include <string_view>
#include <vector>

namespace k3::k3tchup::detail {

struct error;

struct test_result
{
    test_result(std::string_view name_)
        : name(name_) {}

    std::string_view name;
    std::size_t checks = 0;
    std::vector<error> errors{};

    void print_brief(std::ostream& os) const;
    void print_errors(std::ostream& os) const;
};

struct fixture_result
{
    fixture_result(std::string_view name_)
        : name(name_) {}

    std::string_view name;
    std::vector<test_result> passes;
    std::vector<test_result> failures;

    void push_back(test_result&& result);
    void print_brief(std::ostream& os) const;
    void print_errors(std::ostream& os) const;
};

struct check_result
{
    bool compile_time;
    bool run_time;
    operator bool() const noexcept
    {
        return compile_time and run_time;
    }
};

} // namespace k3::k3tchup::detail

#endif // K3_K3TCHUP_DETAIL_RESULT_HPP
