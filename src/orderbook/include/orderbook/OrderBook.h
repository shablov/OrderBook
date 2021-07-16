#pragma once

#include <cstddef>
#include <map>
#include <system_error>

namespace order {

enum class Side { BID, ASK };

struct Element {
  double price;
  double quantity;
  Side side;

  bool is_valid();
};

class Book {
public:
  std::error_code add(Element elem);

  std::error_code change(Element elem);

  std::error_code del(double price);

  double vwap(size_t depth);

private:
  bool checkElement(const Element& elem);
  bool checkDouble(double value);

private:
  std::map<double, Element, std::less<>> _bids;
  std::map<double, Element, std::greater<>> _asks;
};

}  // namespace order