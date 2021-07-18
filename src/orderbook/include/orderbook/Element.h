#pragma once

#include <cmath>

namespace order {

template<typename T>
bool is_equal(T lhs, T rhs) {
  return std::fabs(lhs-rhs) <= std::numeric_limits<T>::epsilon();
}

enum class Side { BID, ASK };

struct Element {
  double price;
  double quantity;
  Side side;

  bool is_valid();
};

}