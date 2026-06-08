// Copyright 2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <k3/k3tchup/detail/context.hpp>
#include <k3/k3tchup/detail/state.hpp>

namespace k3::k3tchup::detail {

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
