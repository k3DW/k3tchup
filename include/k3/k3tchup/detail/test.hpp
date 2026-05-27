// Copyright 2023-2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef K3_K3TCHUP_DETAIL_TEST_HPP
#define K3_K3TCHUP_DETAIL_TEST_HPP

#include <cstddef>
#include <iosfwd>
#include <string_view>

namespace k3::k3tchup::detail {

struct test_result;

class test
{
public:
    std::string_view name() const
    {
        return _name;
    }

    test_result run(std::ostream& os) const;

    using func_type = void(*)();

    template <std::size_t N>
    test(const char(&name)[N], func_type func)
        : _name(name), _func(func) {}

private:
    std::string_view _name; // This is fine because we only construct with a string literal
    func_type _func;
};

} // namespace k3::k3tchup::detail

#endif // K3_K3TCHUP_DETAIL_TEST_HPP
