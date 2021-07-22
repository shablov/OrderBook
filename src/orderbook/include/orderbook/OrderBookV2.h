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


namespace orderV2 {

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
    return ranges::concat_view(ranges::views::all(_bids) | ranges::views::values | ranges::views::reverse,
                               ranges::views::all(_asks) | ranges::views::values | ranges::views::reverse);
  }

private:
  bool checkElement(const Element& elem);
  bool checkDouble(double value);

private:
  std::map<double, Element, std::less<>> _bids;
  std::map<double, Element, std::greater<>> _asks;

  absl::flat_hash_map<double, std::map<double, Element, std::less<>>::iterator> _bidsHashTable;
  absl::flat_hash_map<double, std::map<double, Element, std::greater<>>::iterator> _asksHashTable;
};

}  // namespace orderV2