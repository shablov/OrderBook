#include "orderbook/Element.h"

#include <cmath>


namespace order {

bool Element::is_valid() {
  return (std::fpclassify(price) == FP_NORMAL && price > 0) && (std::fpclassify(quantity) == FP_NORMAL && quantity > 0);
}

bool operator==(const Element& lhs, const Element& rhs) {
  return is_equal(lhs.price, rhs.price) && is_equal(lhs.quantity, rhs.quantity) && lhs.side == rhs.side;
}

}  // namespace order