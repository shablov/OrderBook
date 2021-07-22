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


namespace orderV3_2 {

using order::Element;
using order::Side;

class Book {
public:
  void add(Element elem);

  void change(Element elem);

  void del(double price);

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

  std::vector<Element> _bids;
  std::vector<Element> _asks;

  absl::flat_hash_map<double, uint64_t> _bidsHashTable;
  absl::flat_hash_map<double, uint64_t> _asksHashTable;
};

}  // namespace orderV3_2