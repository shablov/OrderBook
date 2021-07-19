#include "orderbook/OrderBookV2_2.h"

#include <cmath>

#include <range/v3/view/take.hpp>


namespace orderV2_2 {

std::error_code Book::add(Element elem) {
  if (!elem.is_valid()) {
    return std::make_error_code(std::errc::invalid_argument);
  }

  auto& currentOrders = _orders[static_cast<size_t>(elem.side)];
  auto& currentOrdersHashTable = _ordersHashTable[static_cast<size_t>(elem.side)];

  auto emplaceIt = currentOrders.emplace(elem.price, elem);
  currentOrdersHashTable.emplace(elem.price, &emplaceIt.first->second);

  return {};
}

std::error_code Book::change(Element elem) {
  if (!elem.is_valid()) {
    return std::make_error_code(std::errc::invalid_argument);
  }

  auto& currentOrders = _orders[static_cast<size_t>(elem.side)];
  auto& currentOrdersHashTable = _ordersHashTable[static_cast<size_t>(elem.side)];

  if (const auto it = currentOrdersHashTable.find(elem.price); it != currentOrdersHashTable.cend()) {
    it->second->quantity = elem.quantity;
  } else {
    auto emplaceIt = currentOrders.emplace(elem.price, elem);
    currentOrdersHashTable.emplace(elem.price, &emplaceIt.first->second);
  }

  return {};
}

std::error_code Book::del(double price) {
  if (!(std::fpclassify(price) == FP_NORMAL && price > 0)) {
    return std::make_error_code(std::errc::invalid_argument);
  }

  if (_ordersHashTable[static_cast<size_t>(Side::BID)].erase(price)) {
    _orders[static_cast<size_t>(Side::BID)].erase(price);
  }
  if (_ordersHashTable[static_cast<size_t>(Side::ASK)].erase(price)) {
    _orders[static_cast<size_t>(Side::ASK)].erase(price);
  }

  return {};
}

double Book::vwap(size_t depth) {
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