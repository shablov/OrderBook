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


namespace orderV3 {

using order::Element;
using order::Side;

class Book {
public:
  std::error_code add(Element elem);

  std::error_code change(Element elem);

  std::error_code del(double price);

  double vwap(size_t depth);

  auto orders() {
    std::vector<Element> orders(_bids.size() + _asks.size());

    std::partial_sort_copy(_bids.cbegin(), _bids.cend(), orders.begin(), orders.end(), GreateElement{});
    std::partial_sort_copy(_asks.cbegin(), _asks.cend(),
                           orders.begin() + static_cast<std::vector<Element>::difference_type>(_bids.size()),
                           orders.end(), LessElement{});

    return orders;
  }

private:
  bool checkElement(const Element& elem);
  bool checkDouble(double value);

private:
  struct LessElement {
    bool operator()(const Element& lhs, const Element& rhs) { return lhs.price < rhs.price; }
  };

  struct GreateElement {
    bool operator()(const Element& lhs, const Element& rhs) { return rhs.price < lhs.price; }
  };

  std::deque<Element> _bids;
  std::deque<Element> _asks;

  absl::flat_hash_map<double, std::deque<Element>::iterator> _bidsHashTable;
  absl::flat_hash_map<double, std::deque<Element>::iterator> _asksHashTable;
};

}  // namespace orderV3