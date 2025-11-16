#include <k3/k3tchup_main.hpp>
#include <optional>
#include <vector>

auto foo() {
  std::vector<std::optional<int>> out;
  out.emplace_back(4);
  out.emplace_back();
  out.emplace_back(5);
  return out;
}

FIXTURE("my fixture");

TEST("my fixture", "my test") {
  for (const std::optional<int>& x : foo()) {
    EXPECT_THAT([&x] {
      ASSERT_RUN_TIME(x.has_value()) << "optional did not have a value";
      EXPECT_RUN_TIME(*x == 5) << "optional's value was not 5";
    });
  }
}
