#pragma once

#include <cstddef>
#include <system_error>

#include "Element.h"


namespace orderEmpty {

using order::Element;

class Book {
public:
  void add(Element /*elem*/) {}

  void change(Element /*elem*/) {}

  void del(double /*price*/) {}

  double vwap(size_t /*depth*/) { return 1.; }
};

}  // namespace orderEmpty