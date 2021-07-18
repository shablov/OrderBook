#pragma once

#include <cstddef>
#include <map>
#include <system_error>

#ifdef __APPLE__
#include <range/v3/view/all.hpp>
#include <range/v3/view/concat.hpp>
#else
#include <ranges>
#endif

#include "Element.h"


namespace order {

class Book {
public:
  std::error_code add(Element elem);

  std::error_code change(Element elem);

  std::error_code del(double price);

  double vwap(size_t depth);

  auto elements() {
    using namespace std;
    return ranges::concat_view(ranges::views::all(_bids), ranges::views::all(_asks));
  }

private:
  bool checkElement(const Element& elem);
  bool checkDouble(double value);

private:
  std::map<double, Element, std::greater<>> _bids;
  std::map<double, Element, std::less<>> _asks;
};

}  // namespace order