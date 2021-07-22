#pragma once

#include <cstddef>
#include <map>
#include <system_error>
#include <unordered_map>

#include <range/v3/view/all.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/map.hpp>
#include <range/v3/view/reverse.hpp>

#include <absl/container/flat_hash_map.h>

#include "Element.h"


namespace orderV2_2 {

using order::Element;
using order::Side;

class Book {
public:
  void add(Element elem);

  void change(Element elem);

  void del(double price);

  double vwap(size_t depth);

  auto orders() {
    using namespace std;
    return ranges::concat_view(
        ranges::views::all(_orders[static_cast<size_t>(Side::BID)]) | ranges::views::values | ranges::views::reverse,
        ranges::views::all(_orders[static_cast<size_t>(Side::ASK)]) | ranges::views::values);
  }

private:
  bool checkElement(const Element& elem);
  bool checkDouble(double value);

private:
  std::array<std::map<double, Element>, 2> _orders =
      std::array{std::map<double, Element>{}, std::map<double, Element>{}};
  std::array<absl::flat_hash_map<double, Element*>, 2> _ordersHashTable =
      std::array{absl::flat_hash_map<double, Element*>{}, absl::flat_hash_map<double, Element*>{}};
};

}  // namespace orderV2_2