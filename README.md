# k3tchup

`k3tchup` is a unit testing framework putting compile-time testing first. It is broken off from [tok3n](https://github.com/k3DW/tok3n)'s internal testing library into its own standalone project. Because [tok3n](https://github.com/k3DW/tok3n) is fully capable at compile-time, it must also be tested at compile-time.

I gave a talk at C++Now 2024 titled ["Unit Testing an Expression Template Library in C++20"](https://www.youtube.com/watch?v=H4KzM-wDiQw), which explores the space of other popular unit testing libraries, and how they handle compile-time testing. `k3tchup` is designed from the ground up with this as a goal.

At the moment, `k3tchup` is in development and should not be relied upon for any stability.

_Note, all errors are registered and reported at run-time, even the conditions that fail a compile-time check. They do not `static_assert`._

## Building

### Language support

Supports C++20 at a minimum, because of the use of `std::source_location`.

### Use `k3tchup` with CMake

Incorporate this library into your project, and link against the target `k3::k3tchup`.

## Usage examples

See `"./examples/vector_of_optionals.cpp"` for a basic usage example.

### Getting started

Each test file should include `<k3/k3tchup.hpp>`. One of your files should include `<k3/k3tchup_main.hpp>`, which creates the `main()` function for you. Otherwise, `k3::k3tchup::runner` can be used directly, in place of including `<k3/k3tchup_main.hpp>`.

The macros in this library are:

* `FIXTURE(name)`
* `TEST(fixture_name, name)`
* `TEST_CONSTEXPR(fixture_name, name, state_variable)`
* `ASSERT_COMPILE_AND_RUN_TIME(bool)`
* `EXPECT_COMPILE_AND_RUN_TIME(bool)`
* `ASSERT_COMPILE_TIME(bool)`
* `EXPECT_COMPILE_TIME(bool)`
* `ASSERT_RUN_TIME(bool)`
* `EXPECT_RUN_TIME(bool)`
* `ASSERT_STATEFUL(state, bool)`
* `EXPECT_STATEFUL(state, bool)`
* `ASSERT_THAT(packet)`
* `EXPECT_THAT(packet)`

Additionally, the following macros expand to one of the above macros contextually:

* `ASSERT` may be `ASSERT_COMPILE_AND_RUN_TIME` or `ASSERT_STATEFUL`, depending on the number of arguments provided
* `EXPECT` may be `EXPECT_COMPILE_AND_RUN_TIME` or `EXPECT_STATEFUL`, depending on the number of arguments provided

### Fixtures and tests

All tests exist as part of a fixture. The test names and fixture names are string literals.

```cpp
FIXTURE("my fixture");

TEST("my fixture", "my test") {
  // ...
}
```

All tests must be created as part of a fixture.

### Basic checks

`k3tchup` has 6 base-level macros that can be used in any test directly, or in any function that returns `void`.

* `ASSERT_COMPILE_TIME(bool)`
* `ASSERT_RUN_TIME(bool)`
* `ASSERT_COMPILE_AND_RUN_TIME(bool)`
* `EXPECT_COMPILE_TIME(bool)`
* `EXPECT_RUN_TIME(bool)`
* `EXPECT_COMPILE_AND_RUN_TIME(bool)`

Onto the end of each of these macros, you can stream messages with `<<`, as long as this is a valid operation for `std::ostream`.

* The `ASSERT_*` macros will return from the current function upon hitting an error. If used directly in the test, they end the test.
* The `EXPECT_*` macros will log an error, but will continue.

### Nest the checks

`k3tchup` has 2 higher level macros that append to the reported stack trace of the error.

* `ASSERT_THAT(packet)`
* `EXPECT_THAT(packet)`

These macros take a "packet", which is a simplified version of a matcher. The "packet" must be an object that is callable with zero arguments. That is, `packet()` must be valid. The "packet" itself is invoked at run-time, even if the nested checks are invoked at compile-time.

Here is an example.

```cpp
for (const std::optional<int>& x : foo()) {
  EXPECT_THAT([&x] {
    ASSERT_RUN_TIME(x.has_value()) << "optional did not have a value";
    EXPECT_RUN_TIME(*x == 5) << "optional's value was not 5, it was " << *x;
  });
}
```

This is a simple example of a run-time-only test. For each `optional`, `has_value()` is checked. If it fails, the scope is exited, and the 2nd check does not get run. However, because we used `EXPECT_THAT`, then this does not end the test, and instead continues to the next iteration of the loop. If we had instead used `ASSERT_THAT`, the test would end on the first error.

_Note, for any potential future facility with matchers, they would use these macros. A user is able to create their own system of matchers in the meantime. Test parametrization can also be built with these simple facilities._

### Deduced checks

The 6 base-level macros prescribe when their condition is checked, whether compile-time, run-time, or both. If instead, you want to run an entire function or lambda entirely at compile-time and entirely at run-time, this requires a different setup and involves different macros.

* `ASSERT_STATEFUL(state, bool)`
* `EXPECT_STATEFUL(state, bool)`

You can only use this type of check when you have introduced a "state". This is a limitation of mutable state at compile-time in C++.

Option 1: You can use the alternative overloads of `ASSERT_THAT` and `EXPECT_THAT` which take a "packet" that's callable with a `k3::k3tchup::state&` argument.

```cpp
TEST("my fixture", "my test") {
  EXPECT_THAT([](auto& state){
    EXPECT_STATEFUL(state, foo());
  });
}
```

Option 2: You can use the type of test that bundles a state, using `TEST_CONSTEXPR`.

```cpp
TEST_CONSTEXPR("my fixture", "my test", state) {
  EXPECT_STATEFUL(state, foo());
}
```

At this time, you can only stream strings or string_views onto `ASSERT_STATEFUL` and `EXPECT_STATEFUL` calls, unlike the 6 base-level macros where any streamable object is allowed.

_Note that in both of these examples, the body is run at compile-time and at run-time._

### Discovery with ctest

You can opt-in to `ctest` test discovery by calling `k3_k3tchup_discover_tests(${target}, ${assembly_name})`.

### Running the test executable

If you choose not to use `ctest`, you can run the test executable directly.

Here is the help string from the test executable itself.

```
Usage:
  <no-args>            => Same as `run`
  list                 => List all tests
  list <file>          => List all tests and output to the given file
  run                  => Run all tests
  run <fixture>        => Run all tests in the given fixture
  run <fixture> <test> => Run the given test
```

Part of the output from `"./examples/vector_of_optionals.cpp"` ultimately looks like this.

```
Running fixture "vector of optionals"
    Test "run-time" - 5 checks / 2 errors.
    ......
Fixture "vector of optionals" - ? tests / ? failures.

================================

? tests failed.

Fixture "vector of optionals"
Test "run-time" failed with 2 errors.
[Run-time fatal error]
    at path\to\k3tchup\examples\vector_of_optionals.cpp:21
    at path\to\k3tchup\examples\vector_of_optionals.cpp:21
    optional did not have a value
[Run-time non-fatal error]
    at path\to\k3tchup\examples\vector_of_optionals.cpp:21
    at path\to\k3tchup\examples\vector_of_optionals.cpp:21
    optional's value was not 5, it was 4
......
```
