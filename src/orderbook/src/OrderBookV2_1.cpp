#include "orderbook/OrderBookV2_1.h"

#include <cmath>


namespace orderV2_1 {

std::error_code Book::add(Element elem) {
  if (!elem.is_valid()) {
    return std::make_error_code(std::errc::invalid_argument);
  }

  if (elem.side == Side::ASK) {
    auto emplaceIt = _asks.emplace(elem.price, elem);
    _asksHashTable.emplace(elem.price, std::ref(emplaceIt.first->second));
  } else if (elem.side == Side::BID) {
    auto emplaceIt = _bids.emplace(elem.price, elem);
    _bidsHashTable.emplace(elem.price, std::ref(emplaceIt.first->second));
  }

  return {};
}

std::error_code Book::change(Element elem) {
  if (!elem.is_valid()) {
    return std::make_error_code(std::errc::invalid_argument);
  }

  if (elem.side == Side::ASK) {
    if (const auto it = _asksHashTable.find(elem.price); it != _asksHashTable.cend()) {
      it->second.get().quantity = elem.quantity;
    } else {
      auto emplaceIt = _asks.emplace(elem.price, elem);
      _asksHashTable.emplace(elem.price, std::ref(emplaceIt.first->second));
    }
  } else if (elem.side == Side::BID) {
    if (const auto it = _bidsHashTable.find(elem.price); it != _bidsHashTable.cend()) {
      it->second.get().quantity = elem.quantity;
    } else {
      auto emplaceIt = _bids.emplace(elem.price, elem);
      _bidsHashTable.emplace(elem.price, std::ref(emplaceIt.first->second));
    }
  }

  return {};
}

std::error_code Book::del(double price) {
  if (!(std::fpclassify(price) == FP_NORMAL && price > 0)) {
    return std::make_error_code(std::errc::invalid_argument);
  }

  _asks.erase(price);
  _asksHashTable.erase(price);
  _bids.erase(price);
  _bidsHashTable.erase(price);

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

}  // namespace orderV2_1