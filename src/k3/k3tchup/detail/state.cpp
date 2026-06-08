// Copyright 2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <k3/k3tchup/detail/context.hpp>
#include <k3/k3tchup/detail/state.hpp>

namespace k3::k3tchup::detail {

namespace {

// There is a big difference between storing a lambda in a variable,
// instead of passing the lambda directly into the `EXPECT_THAT` macro.
// In the latter case, MSVC may give different results from a
// `std::source_location::current().function_name()` inside the lambda.
// This is not a bug in MSVC, rather just a difference in the way the
// name of a lambda is displayed: "lambda_1", "lambda_2", etc. This
// function is needed to normalize all of these into "lambda_x".
#ifdef _MSC_VER
std::string normalize_function_name(std::string s) {
    constexpr std::string_view prefix = "<lambda_";
    constexpr char suffix = '>';
    std::size_t pos = 0;
    while ((pos = s.find(prefix, pos)) != std::string::npos) {
        std::size_t digit_start = pos + prefix.size();
        std::size_t i = digit_start;
        while (i < s.size() && s[i] >= '0' && s[i] <= '9') {
            ++i;
        }
        if (i > digit_start && i < s.size() && s[i] == suffix) {
            s.replace(digit_start, i - digit_start, 1, 'x');
            pos = digit_start + 2; // Move past the suffix
        } else {
            pos = digit_start;
        }
    }
    return s;
}
#else
std::string_view normalize_function_name(const char* name) {
    return name;
}
#endif

} // namespace

bool operator==(const state_error& lhs, const state_error& rhs) {
    return
        lhs.index == rhs.index and
        lhs.fatality == rhs.fatality and
        lhs.message == rhs.message and
        lhs.location.line() == rhs.location.line() and
        lhs.location.column() == rhs.location.column() and
        std::string_view(lhs.location.file_name()) == std::string_view(rhs.location.file_name()) and
        normalize_function_name(lhs.location.function_name()) == normalize_function_name(rhs.location.function_name());
}

void state_accessor::report(const state& ct, const state& rt) {
    const auto num_checks = ct.num_checks > rt.num_checks ? ct.num_checks : rt.num_checks;
    auto ct_it = ct.errors.value().begin();
    auto rt_it = rt.errors.value().begin();
    const auto ct_end = ct.errors.value().end();
    const auto rt_end = rt.errors.value().end();
    for (std::size_t i = 0; i != num_checks; ++i) {
        context::check(true, true);
        if (ct_it != ct_end and rt_it != rt_end and ct_it->index == i and rt_it->index == i) {
            if (*ct_it == *rt_it) {
                context::add_error({ .compile_time = false, .run_time = false}, ct_it->fatality, ct_it->location)
                    << ct_it->message;
                ++ct_it;
                ++rt_it;
            } else {
                context::add_error({ .compile_time = false, .run_time = true }, ct_it->fatality, ct_it->location)
                    << ct_it->message;
                context::add_error({ .compile_time = true, .run_time = false }, rt_it->fatality, rt_it->location)
                    << rt_it->message;
                ++ct_it;
                ++rt_it;
            }
        } else if (ct_it != ct_end and ct_it->index == i) {
            context::add_error({ .compile_time = false, .run_time = true }, ct_it->fatality, ct_it->location)
                << ct_it->message;
            ++ct_it;
        } else if (rt_it != rt_end and rt_it->index == i) {
            context::add_error({ .compile_time = true, .run_time = false }, rt_it->fatality, rt_it->location)
                << rt_it->message;
            ++rt_it;
        }
    }
    if (ct_it != ct_end) {
        throw; // logic error
    }
    if (rt_it != rt_end) {
        throw; // logic error
    }
}

} // namespace k3::k3tchup::detail
