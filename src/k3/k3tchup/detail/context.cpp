// Copyright 2023-2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <k3/k3tchup.hpp>
#include <iomanip>
#include <iostream>
#include <stdexcept>

namespace k3::k3tchup::detail {

check_result context::check(bool compile_time, bool run_time)
{
    ++_current_result->checks;
    return {
        .compile_time = compile_time,
        .run_time = run_time,
    };
}

std::size_t context::total_errors()
{
    return _total_errors;
}

message_streaming_context context::add_error(check_result result, error_fatality fatality, std::source_location location)
{
    if (result)
    {
        throw std::logic_error("At least one of compile_time or run_time must be false.");
    }

    std::vector<std::source_location> trace;
    trace = _trace;
    trace.push_back(std::move(location));
    ++_total_errors;

    const error_time time =
        (not result.compile_time and not result.run_time) ? error_time::both :
        (not result.compile_time) ? error_time::compile_time : error_time::run_time;
    error& e = _current_result->errors.emplace_back(time, fatality, std::move(trace));
    return message_streaming_context{e.message};
}

test_result_context::test_result_context(test_result& result)
{
    if (context::_current_result != nullptr)
    {
        throw std::logic_error("Nested test_result_context is not allowed.");
    }
    context::_current_result = &result;
}

test_result_context::~test_result_context()
{
    context::_current_result = nullptr;
}

} // namespace k3::k3tchup::detail
