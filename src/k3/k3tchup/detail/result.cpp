// Copyright 2023-2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <k3/k3tchup.hpp>
#include <iomanip>
#include <iostream>
#include <stdexcept>

namespace k3::k3tchup::detail {

void test_result::print_brief(std::ostream& os) const
{
    os
        << "    Test " << std::quoted(name) << " - "
        << checks << " check" << (checks == 1 ? "" : "s") << " / " << errors.size()
        << " error" << (errors.size() == 1 ? "" : "s") << ".\n";
}

void test_result::print_errors(std::ostream& os) const
{
    os << "Test " << std::quoted(name) << " failed with "
        << errors.size() << " error" << (errors.size() == 1 ? "" : "s") << ".\n";
    for (const error& e : errors)
    {
        print(os, e);
    }
}

void fixture_result::push_back(test_result&& result)
{
    if (result.errors.empty())
        passes.push_back(std::move(result));
    else
        failures.push_back(std::move(result));
}

void fixture_result::print_brief(std::ostream& os) const
{
    const std::size_t num_tests = passes.size() + failures.size();
    os
        << "Fixture " << std::quoted(name) << " - "
        << num_tests << " test" << (num_tests == 1 ? "" : "s") << " / "
        << failures.size() << " failure" << (failures.size() == 1 ? "" : "s") << ".\n\n";
}

void fixture_result::print_errors(std::ostream& os) const
{
    os << "Fixture " << std::quoted(name) << "\n";
    for (const test_result& result : failures)
    {
        result.print_errors(os);
    }
    os << "\n";
}

} // namespace k3::k3tchup::detail
