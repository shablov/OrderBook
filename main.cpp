#include <iostream>
#include <vector>

#include "OrderBook.h"

std::vector<Element> get_snapshot() {
  return {Element{37.38, 11380, Side::BID},
          Element{37.39, 19601, Side::BID},
          Element{37.40, 19760, Side::BID},
          Element{37.41, 37236, Side::BID},
          Element{37.42, 65811, Side::BID},
          Element{37.43, 367731, Side::BID},
          Element{37.44, 2056, Side::BID},
          Element{37.45, 87281, Side::BID},
          Element{37.46, 72701, Side::BID},
          Element{37.47, 49121, Side::BID},
          Element{37.28, 176828, Side::ASK},
          Element{37.29, 142334, Side::ASK},
          Element{37.30, 155878, Side::ASK},
          Element{37.31, 117585, Side::ASK},
          Element{37.32, 118253, Side::ASK},
          Element{37.33, 209282, Side::ASK},
          Element{37.34, 385363, Side::ASK},
          Element{37.35, 340823, Side::ASK},
          Element{37.36, 153408, Side::ASK},
          Element{37.37, 187456, Side::ASK}};
}

bool is_equal(double lhs, double rhs, double epsilon) {
  auto diff = lhs - rhs;
  return (diff < epsilon) && (-diff < epsilon);
}

int main() {
  OrderBook book;
  auto snapshot = get_snapshot();
  for (auto level : snapshot) {
    book.add(level);
  }

  if (auto vwap = book.vwap(5); !is_equal(vwap, 37.3543, 0.0001)) {
    std::cout << vwap << std::endl;
    return 1;
  }

  return 0;
}
