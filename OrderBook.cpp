#include "OrderBook.h"

#include <iostream>

void OrderBook::add(Element elem) {
  if (elem.side == Side::ASK) {
    _asks.emplace(elem.price, elem);
  }

  if (elem.side == Side::BID) {
    _bids.emplace(elem.price, elem);
  }
}

void OrderBook::change(Element elem) {
  if (elem.side == Side::ASK) {
    if (auto it = _asks.find(elem.price); it != _asks.end()) {
      it->second = elem;
    }
  }

  if (elem.side == Side::BID) {
    if (auto it = _bids.find(elem.price); it != _bids.end()) {
      it->second = elem;
    }
  }
}

void OrderBook::del(double price) {
  _asks.erase(price);
  _bids.erase(price);
}

double OrderBook::vwap(size_t depth) {
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
