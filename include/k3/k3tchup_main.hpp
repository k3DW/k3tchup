// Copyright 2025 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <k3/k3tchup.hpp>

int main(const int argc, const char* const* const argv) {
  return k3::k3tchup::runner::get().exec(argc, argv);
}
