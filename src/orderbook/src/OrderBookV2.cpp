#include "orderbook/OrderBookV2.h"

#include <cmath>

#include <range/v3/view/take.hpp>


namespace orderV2 {

void Book::add(Element elem) {
  /// Unite orders as NYSE exchange
  if (elem.side == Side::ASK) {
    if (const auto it = _asksHashTable.find(elem.price); it != _asksHashTable.cend()) {
      it->second->second.quantity += elem.quantity;
    } else {
      auto emplaceIt = _asks.emplace(elem.price, elem);
      _asksHashTable.emplace(elem.price, emplaceIt.first);
    }
  } else if (elem.side == Side::BID) {
    if (const auto it = _bidsHashTable.find(elem.price); it != _bidsHashTable.cend()) {
      it->second->second.quantity += elem.quantity;
    } else {
      auto emplaceIt = _bids.emplace(elem.price, elem);
      _bidsHashTable.emplace(elem.price, emplaceIt.first);
    }
  }
}

void Book::change(Element elem) {
  if (elem.side == Side::ASK) {
    if (const auto it = _asksHashTable.find(elem.price); it != _asksHashTable.cend()) {
      it->second->second.quantity = elem.quantity;
    } else {
      auto emplaceIt = _asks.emplace(elem.price, elem);
      _asksHashTable.emplace(elem.price, emplaceIt.first);
    }
  } else if (elem.side == Side::BID) {
    if (const auto it = _bidsHashTable.find(elem.price); it != _bidsHashTable.cend()) {
      it->second->second.quantity = elem.quantity;
    } else {
      auto emplaceIt = _bids.emplace(elem.price, elem);
      _bidsHashTable.emplace(elem.price, emplaceIt.first);
    }
  }
}

void Book::del(double price) {
  _asks.erase(price);
  _asksHashTable.erase(price);
  _bids.erase(price);
  _bidsHashTable.erase(price);
}

double Book::vwap(size_t depth) {
  if (_bids.empty() && _asks.empty()) {
    return 0.;
  }

  double sum = 0;
  double volumes = 0;

  for (const auto& [price, elem] : _bids | ranges::views::take(depth)) {
    sum += elem.price * elem.quantity;
    volumes += elem.quantity;
  }

  for (const auto& [price, elem] : _asks | ranges::views::take(depth)) {
    sum += elem.price * elem.quantity;
    volumes += elem.quantity;
  }

  return sum / volumes;
}

}  // namespace orderV2