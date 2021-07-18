#pragma once

#include <cstddef>
#include <map>
#include <system_error>
#include <unordered_map>

#include <range/v3/view/all.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/reverse.hpp>

#include <absl/container/flat_hash_map.h>

#include "Element.h"


namespace orderV2_1 {

using order::Element;
using order::Side;

class Book {
public:
  std::error_code add(Element elem);

  std::error_code change(Element elem);

  std::error_code del(double price);

  double vwap(size_t depth);

  auto elements() {
    using namespace std;
    return ranges::concat_view(ranges::views::all(_bids) | ranges::views::reverse,
                               ranges::views::all(_asks) | ranges::views::reverse);
  }

private:
  bool checkElement(const Element& elem);
  bool checkDouble(double value);

private:
  std::map<double, Element, std::less<>> _bids;
  std::map<double, Element, std::greater<>> _asks;

  absl::flat_hash_map<double, std::reference_wrapper<Element>> _bidsHashTable;
  absl::flat_hash_map<double, std::reference_wrapper<Element>> _asksHashTable;
};

}  // namespace orderV2_1