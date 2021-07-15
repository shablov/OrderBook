#pragma once

#include <cstddef>
#include <map>

namespace order {

enum class Side { BID, ASK };

struct Element {
  double price;
  double quantity;
  Side side;
};

class Book {
public:
  void add(Element elem);

  void change(Element elem);

  void del(double price);

  double vwap(size_t depth);

private:
  std::map<double, Element, std::less<>> _bids;
  std::map<double, Element, std::greater<>> _asks;
};

}  // namespace order