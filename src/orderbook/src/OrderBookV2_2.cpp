#include "orderbook/OrderBookV2_2.h"

#include <cmath>

#include <range/v3/view/take.hpp>


namespace orderV2_2 {

void Book::add(Element elem) {
  /// Unite orders as NYSE exchange
  auto& currentOrders = _orders[static_cast<size_t>(elem.side)];
  auto& currentOrdersHashTable = _ordersHashTable[static_cast<size_t>(elem.side)];

  if (const auto it = currentOrdersHashTable.find(elem.price); it != currentOrdersHashTable.cend()) {
    it->second->quantity += elem.quantity;
  } else {
    auto emplaceIt = currentOrders.emplace(elem.price, elem);
    currentOrdersHashTable.emplace(elem.price, &emplaceIt.first->second);
  }
}

void Book::change(Element elem) {
  auto& currentOrders = _orders[static_cast<size_t>(elem.side)];
  auto& currentOrdersHashTable = _ordersHashTable[static_cast<size_t>(elem.side)];

  if (const auto it = currentOrdersHashTable.find(elem.price); it != currentOrdersHashTable.cend()) {
    it->second->quantity = elem.quantity;
  } else {
    auto emplaceIt = currentOrders.emplace(elem.price, elem);
    currentOrdersHashTable.emplace(elem.price, &emplaceIt.first->second);
  }
}

void Book::del(double price) {
  if (_ordersHashTable[static_cast<size_t>(Side::BID)].erase(price)) {
    _orders[static_cast<size_t>(Side::BID)].erase(price);
  }
  if (_ordersHashTable[static_cast<size_t>(Side::ASK)].erase(price)) {
    _orders[static_cast<size_t>(Side::ASK)].erase(price);
  }
}

double Book::vwap(size_t depth) {
  if (_orders[static_cast<size_t>(Side::BID)].empty() && _orders[static_cast<size_t>(Side::ASK)].empty()) {
    return 0.;
  }

  double sum = 0;
  double volumes = 0;

  for (const auto& [price, elem] : _orders[static_cast<size_t>(Side::BID)] | ranges::views::take(depth)) {
    sum += elem.price * elem.quantity;
    volumes += elem.quantity;
  }

  for (const auto& [price, elem] :
       _orders[static_cast<size_t>(Side::ASK)] | ranges::views::reverse | ranges::views::take(depth)) {
    sum += elem.price * elem.quantity;
    volumes += elem.quantity;
  }

  return sum / volumes;
}

}  // namespace orderV2_2