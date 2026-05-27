// Copyright 2023-2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef K3_K3TCHUP_DETAIL_FIXTURE_HPP
#define K3_K3TCHUP_DETAIL_FIXTURE_HPP

#include <iosfwd>
#include <string>
#include <string_view>
#include <unordered_map>
#include <k3/k3tchup/test.hpp>

namespace k3::k3tchup::detail {

struct fixture_result;

class fixture
{
public:
    void add_test(test&& t);

    std::string_view name() const
    {
        return _name;
    }

    fixture_result run(std::ostream& os) const;
    fixture_result run(std::ostream& os, std::string_view test_name) const;

    void list(std::ostream& os) const;

protected:
    fixture(std::string name)
        : _name(std::move(name))
    {}

    template <std::size_t hash>
    static fixture& global(std::string name) {
        static fixture f(std::move(name));
        return f;
    }

private:
    std::string _name;
    std::unordered_map<std::string_view, test> _tests;
};

} // namespace k3::k3tchup::detail

#endif // K3_K3TCHUP_DETAIL_FIXTURE_HPP
