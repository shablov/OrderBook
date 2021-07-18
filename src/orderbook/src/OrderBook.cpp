#include "orderbook/OrderBook.h"

#include <cmath>
#include <iostream>


namespace order {

std::error_code Book::add(Element elem) {
  if (!elem.is_valid()) {
    return std::make_error_code(std::errc::invalid_argument);
  }

  if (elem.side == Side::ASK) {
    _asks.emplace(elem.price, elem);
  } else if (elem.side == Side::BID) {
    _bids.emplace(elem.price, elem);
  }

  return {};
}

std::error_code Book::change(Element elem) {
  if (!elem.is_valid()) {
    return std::make_error_code(std::errc::invalid_argument);
  }

  if (elem.side == Side::ASK) {
    if (auto it = _asks.find(elem.price); it != _asks.end()) {
      it->second = elem;
    } else {
      _asks.emplace(elem.price, elem);
    }
  } else if (elem.side == Side::BID) {
    if (auto it = _bids.find(elem.price); it != _bids.end()) {
      it->second = elem;
    } else {
      _bids.emplace(elem.price, elem);
    }
  }

  return {};
}

std::error_code Book::del(double price) {
  if (!(std::fpclassify(price) == FP_NORMAL && price > 0)) {
    return std::make_error_code(std::errc::invalid_argument);
  }

  _asks.erase(price);
  _bids.erase(price);

  return {};
}

double Book::vwap(size_t depth) {
  double sum = 0;
  double volumes = 0;

  size_t count = 0;
  for (auto it = _bids.cbegin(); it != _bids.cend() && count < depth; ++it, ++count) {
    const auto& [price, elem] = *it;
    sum += price * elem.quantity;
    volumes += elem.quantity;
  }

  count = 0;
  for (auto it = _asks.cbegin(); it != _asks.cend() && count < depth; ++it, ++count) {
    const auto& [price, elem] = *it;
    sum += price * elem.quantity;
    volumes += elem.quantity;
  }

  return sum / volumes;
}

}  // namespace order