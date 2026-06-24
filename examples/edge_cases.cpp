// Copyright 2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <k3/k3tchup_main.hpp>

FIXTURE("edge cases");

// Because we can call a check from within a function template,
// the macro must not cause any generic lambdas to be invoked
// at compile-time, due to a Clang bug.
// https://github.com/llvm/llvm-project/issues/35052
// https://github.com/llvm/llvm-project/issues/205218
template <class T>
void same_size_as_int() {
    EXPECT_COMPILE_TIME(sizeof(T) == sizeof(int));
}

TEST("edge cases", "check inside template") {
    EXPECT_THAT(same_size_as_int<int>);
    EXPECT_THAT(same_size_as_int<const int>);
    EXPECT_THAT(same_size_as_int<int&>);
    EXPECT_THAT(same_size_as_int<const int&>);
    struct S { int& i; };
    EXPECT_THAT(same_size_as_int<S>);
}
