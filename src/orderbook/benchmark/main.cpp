#include <iostream>
#include <vector>

#include "orderbook/OrderBook.h"


std::vector<order::Element> get_snapshot() {
  return {order::Element{37.38, 11380, order::Side::BID},  order::Element{37.39, 19601, order::Side::BID},
          order::Element{37.40, 19760, order::Side::BID},  order::Element{37.41, 37236, order::Side::BID},
          order::Element{37.42, 65811, order::Side::BID},  order::Element{37.43, 367731, order::Side::BID},
          order::Element{37.44, 2056, order::Side::BID},   order::Element{37.45, 87281, order::Side::BID},
          order::Element{37.46, 72701, order::Side::BID},  order::Element{37.47, 49121, order::Side::BID},
          order::Element{37.28, 176828, order::Side::ASK}, order::Element{37.29, 142334, order::Side::ASK},
          order::Element{37.30, 155878, order::Side::ASK}, order::Element{37.31, 117585, order::Side::ASK},
          order::Element{37.32, 118253, order::Side::ASK}, order::Element{37.33, 209282, order::Side::ASK},
          order::Element{37.34, 385363, order::Side::ASK}, order::Element{37.35, 340823, order::Side::ASK},
          order::Element{37.36, 153408, order::Side::ASK}, order::Element{37.37, 187456, order::Side::ASK}};
}

bool is_equal(double lhs, double rhs, double epsilon) {
  auto diff = lhs - rhs;
  return (diff < epsilon) && (-diff < epsilon);
}

int main() {
  order::Book book;
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




