// Copyright 2023-2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef K3_K3TCHUP_DETAIL_CONTEXT_HPP
#define K3_K3TCHUP_DETAIL_CONTEXT_HPP

#include <cstddef>
#include <ostream>
#include <source_location>
#include <sstream>
#include <string>
#include <vector>
#include <k3/k3tchup/detail/error.hpp>
#include <k3/k3tchup/detail/result.hpp>

namespace k3::k3tchup::detail {

class test_result_context;
class trace_context;
class message_streaming_context;

class context
{
    friend class test_result_context;
    friend class trace_context;

public:
    static message_streaming_context add_error(check_result result, error_fatality fatality, std::source_location location = std::source_location::current());

    static check_result check(bool compile_time, bool run_time);

    static std::size_t total_errors();

private:
    static inline test_result* _current_result = nullptr;
    static inline std::vector<std::source_location> _trace;

    static inline std::size_t _total_errors = 0;
};

class test_result_context
{
public:
    [[nodiscard]] test_result_context(test_result& result);
    ~test_result_context();
    test_result_context(const test_result_context&) = delete;
    test_result_context& operator=(const test_result_context&) = delete;
};

class trace_context
{
public:
    [[nodiscard]] trace_context(std::source_location location = std::source_location::current())
    {
        context::_trace.push_back(std::move(location));
    }
    ~trace_context()
    {
        context::_trace.pop_back();
    }
    trace_context(const trace_context&) = delete;
    trace_context& operator=(const trace_context&) = delete;
};

class message_streaming_context
{
public:
    message_streaming_context(std::string& message)
        : _message(&message)
    {}
    ~message_streaming_context()
    {
        *_message = std::move(_stream).str();
    }
    message_streaming_context(const message_streaming_context&) = delete;
    message_streaming_context& operator=(const message_streaming_context&) = delete;
    template <class T>
    friend message_streaming_context&& operator<<(message_streaming_context&& ctx, T&& value)
    {
        ctx._stream << std::forward<T>(value);
        return std::move(ctx);
    }
private:
    std::ostringstream _stream;
    std::string* _message;
};

} // namespace k3::k3tchup::detail

#endif // K3_K3TCHUP_DETAIL_CONTEXT_HPP
