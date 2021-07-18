#pragma once

#include <cstddef>
#include <system_error>

#include "Element.h"


namespace orderEmpty {

using order::Element;

class Book {
public:
  std::error_code add(Element /*elem*/) { return {}; }

  std::error_code change(Element /*elem*/) { return {}; }

  std::error_code del(double /*price*/) { return {}; }

  double vwap(size_t /*depth*/) { return 1.; }
};

}  // namespace orderEmpty