// Copyright 2025-2026 Braden Ganetsky
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <k3/k3tchup_main.hpp>
#include <optional>
#include <vector>

constexpr auto vec() {
  std::vector<std::optional<int>> out;
  out.emplace_back(5);
  out.emplace_back();
  out.emplace_back(4);
  return out;
}

FIXTURE("vector of optionals");

// This test will emit 1 error for each of the failing optionals in the vector.
// Upon encountering a disengaged optional, the body of the `EXPECT_THAT` will
// return early, and skip the second check. However, the test will not end, and
// the loop will continue.
TEST("vector of optionals", "run-time") {
  for (const std::optional<int>& x : vec()) {
    EXPECT_THAT([&x] {
      ASSERT_RUN_TIME(x.has_value()) << "optional did not have a value";
      EXPECT_RUN_TIME(*x == 5) << "optional's value was not 5, it was " << *x;
    });
  }
}

// This test will only emit errors until encountering the first disengaged
// optional, when it immediately ends the test.
TEST("vector of optionals", "run-time short-circuit") {
  for (const std::optional<int>& x : vec()) {
    ASSERT_RUN_TIME(x.has_value()) << "optional did not have a value";
    EXPECT_RUN_TIME(*x == 5) << "optional's value was not 5, it was " << *x;
  }
}

// This test will end upon encountering its first error. The `ASSERT_THAT` macro
// turns any failure in its body into a fatal error in its own scope.
TEST("vector of optionals", "run-time short-circuit alt") {
  for (const std::optional<int>& x : vec()) {
    ASSERT_THAT([&x] {
      ASSERT_RUN_TIME(x.has_value()) << "optional did not have a value";
      EXPECT_RUN_TIME(*x == 5) << "optional's value was not 5, it was " << *x;
    });
  }
}

// This test will run the lambda once at compile-time and once at run-time, which
// prioritizes the performance of the test. Testing at compile-time and run-time
// always requires that the computation is done twice, but we can limit this to
// _only_ twice. This sacrifices message readability, since this example doesn't
// have any specifics for what went wrong upon hitting an error.
TEST("vector of optionals", "compile-time basic") {
  EXPECT_COMPILE_AND_RUN_TIME(([]() -> bool {
    bool success = true;
    for (const std::optional<int>& x : vec()) {
      success = success && x.has_value();
      success = success && (*x == 5);
    }
    return success;
  }()));
}

// Without using a mechanism to iterate over the vector, this test does it with
// an explicit index. This test does much more computation than the "basic" test
// above, but the error messages are more descriptive. It's necessary to call
// `vec()` frequently, since we can't store a std::vector at compile-time.
// Note: Here, you can simply write an `ASSERT_*` followed by an `EXPECT_*` as
// straight-line code, even though the first condition being false should cause
// the second condition to hard fail the compile. The key is that every
// compile-time check is now secretly dependent on there not having been a fatal
// compile-time error previously in the same function.
TEST("vector of optionals", "compile-time manual iteration") {
  auto one_element = []<std::size_t I>(std::integral_constant<std::size_t, I>) {
    ASSERT_COMPILE_AND_RUN_TIME(vec()[I].has_value()) << "optional did not have a value";
    EXPECT_COMPILE_AND_RUN_TIME(*vec()[I] == 5) << "optional's value was not 5, it was " << *vec()[I];
  };
  one_element(std::integral_constant<std::size_t, 0>{});
  one_element(std::integral_constant<std::size_t, 1>{});
  one_element(std::integral_constant<std::size_t, 2>{});
}

TEST("vector of optionals", "stateful checks") {
  EXPECT_THAT_2([](k3::k3tchup::state& s) {
    for (const auto& o : vec()) {
      [&] {
        ASSERT_DEDUCE(s, o.has_value()) << "optional did not have a value";
        EXPECT_DEDUCE(s, *o == 5) << "optional's value was not 5";
      }();
    }
  });
}
