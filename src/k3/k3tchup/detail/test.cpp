// Copyright 2023-2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <k3/k3tchup.hpp>
#include <iomanip>
#include <iostream>

namespace k3::k3tchup::detail {

test_result test::run(std::ostream& os) const
{
    test_result result(_name);
    {
        test_result_context ctx(result);
        _func();
    }
    result.print_brief(os);
    return result;
}

} // namespace k3::k3tchup::detail
