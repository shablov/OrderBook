#include "orderbook/Element.h"

#include <cmath>


namespace order {

bool Element::is_valid() {
  return (std::fpclassify(price) == FP_NORMAL && price > 0) && (std::fpclassify(quantity) == FP_NORMAL && quantity > 0);
}

}