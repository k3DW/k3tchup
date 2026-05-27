// Copyright 2023-2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef K3_K3TCHUP_DETAIL_VOID_ASSIGNER_HPP
#define K3_K3TCHUP_DETAIL_VOID_ASSIGNER_HPP

namespace k3::k3tchup::detail {

class void_assigner
{
public:
    void_assigner() = default;
    void operator=(auto&&) const {}
};

} // namespace k3::k3tchup::detail

#endif // K3_K3TCHUP_DETAIL_VOID_ASSIGNER_HPP
