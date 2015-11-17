#include <sliding_window.hpp>

#include <vector>
#include <array>
#include <string>
#include <utility>

#include "helpers.hpp"
#include "catch.hpp"

using iter::sliding_window;
using Vec = const std::vector<int>;

TEST_CASE("sliding_window: window of size 3", "[sliding_window]") {
  Vec ns = {10, 20, 30, 40, 50};
  auto sw = sliding_window(ns, 3);
  auto it = std::begin(sw);
  REQUIRE(it != std::end(sw));
  {
    Vec v(std::begin(*it), std::end(*it));
    Vec vc = {10, 20, 30};
    REQUIRE(v == vc);
  }
  ++it;
  REQUIRE(it != std::end(sw));
  {
    Vec v(std::begin(*it), std::end(*it));
    Vec vc = {20, 30, 40};
    REQUIRE(v == vc);
  }
  ++it;
  REQUIRE(it != std::end(sw));
  {
    Vec v(std::begin(*it), std::end(*it));
    Vec vc = {30, 40, 50};
    REQUIRE(v == vc);
  }
  ++it;
  REQUIRE(it == std::end(sw));
}

TEST_CASE("sliding window: oversized window is empty", "[sliding_window]") {
  Vec ns = {10, 20, 30};
  auto sw = sliding_window(ns, 5);
  REQUIRE(std::begin(sw) == std::end(sw));
}

TEST_CASE("sliding window: window size == len(iterable)", "[sliding_window]") {
  Vec ns = {10, 20, 30};
  auto sw = sliding_window(ns, 3);
  auto it = std::begin(sw);
  REQUIRE(it != std::end(sw));

  Vec v(std::begin(*it), std::end(*it));

  REQUIRE(ns == v);
  ++it;
  REQUIRE(it == std::end(sw));
}

TEST_CASE("sliding window: empty iterable is empty", "[sliding_window]") {
  Vec ns{};
  auto sw = sliding_window(ns, 1);
  REQUIRE(std::begin(sw) == std::end(sw));
}

TEST_CASE("sliding window: window size of 1", "[sliding_window]") {
  Vec ns = {10, 20, 30};
  auto sw = sliding_window(ns, 1);
  auto it = std::begin(sw);
  REQUIRE(*std::begin(*it) == 10);
  ++it;
  REQUIRE(*std::begin(*it) == 20);
  ++it;
  REQUIRE(*std::begin(*it) == 30);
  ++it;
  REQUIRE(it == std::end(sw));
}

TEST_CASE("sliding window: window size of 0", "[sliding_window]") {
  Vec ns = {10, 20, 30};
  auto sw = sliding_window(ns, 0);
  REQUIRE(std::begin(sw) == std::end(sw));
}

TEST_CASE(
    "sliding window: moves rvalues and binds to lvalues", "[sliding_window]") {
  itertest::BasicIterable<int> bi{1, 2};
  sliding_window(bi, 1);
  REQUIRE_FALSE(bi.was_moved_from());
  sliding_window(std::move(bi), 1);
  REQUIRE(bi.was_moved_from());
}

TEST_CASE("sliding window: doesn't copy elements", "[sliding_window]") {
  constexpr std::array<itertest::SolidInt, 3> arr{{{6}, {7}, {8}}};
  for (auto&& i : sliding_window(arr, 1)) {
    (void)*std::begin(i);
  }
}

TEST_CASE("sliding_window: iterator meets requirements", "[sliding_window]") {
  std::string s{"abcdef"};
  auto c = sliding_window(s, 2);
  REQUIRE(itertest::IsIterator<decltype(std::begin(c))>::value);
}

template <typename T>
using ImpT = decltype(sliding_window(std::declval<T>(), 1));
TEST_CASE(
    "sliding_window: has correct ctor and assign ops", "[sliding_window]") {
  REQUIRE(itertest::IsMoveConstructibleOnly<ImpT<std::string&>>::value);
  REQUIRE(itertest::IsMoveConstructibleOnly<ImpT<std::string>>::value);
}
