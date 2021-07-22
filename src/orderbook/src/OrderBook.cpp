#include "orderbook/OrderBook.h"

#include <cmath>

#include <range/v3/view/take.hpp>


namespace order {

void Book::add(Element elem) {
  /// Unite orders as NYSE exchange
  if (elem.side == Side::ASK) {
    if (auto it = _asks.find(elem.price); it != _asks.end()) {
      it->second.quantity += elem.quantity;
    } else {
      _asks.emplace(elem.price, elem);
    }
  } else if (elem.side == Side::BID) {
    if (auto it = _bids.find(elem.price); it != _bids.end()) {
      it->second.quantity += elem.quantity;
    } else {
      _bids.emplace(elem.price, elem);
    }
  }
}

void Book::change(Element elem) {
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
}

void Book::del(double price) {
  _asks.erase(price);
  _bids.erase(price);
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

}  // namespace order