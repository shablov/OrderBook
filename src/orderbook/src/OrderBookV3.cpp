#include "orderbook/OrderBookV3.h"

#include <cmath>

#include <range/v3/view/take.hpp>


namespace orderV3 {

std::error_code Book::add(Element elem) {
  if (!elem.is_valid()) {
    return std::make_error_code(std::errc::invalid_argument);
  }

  if (elem.side == Side::ASK) {
    auto emplaceIt = _asks.emplace(_asks.cend(), elem);
    _asksHashTable.emplace(elem.price, emplaceIt);
  } else if (elem.side == Side::BID) {
    auto emplaceIt = _bids.emplace(_bids.cend(), elem);
    _bidsHashTable.emplace(elem.price, emplaceIt);
  }

  return {};
}

std::error_code Book::change(Element elem) {
  if (!elem.is_valid()) {
    return std::make_error_code(std::errc::invalid_argument);
  }

  switch (elem.side) {
  case Side::BID:
    if (const auto foundIt = _bidsHashTable.find(elem.price); foundIt != _bidsHashTable.cend()) {
      foundIt->second->quantity = elem.quantity;
    } else {
      auto emplaceIt = _bids.emplace(_bids.cend(), elem);
      _bidsHashTable.emplace(elem.price, emplaceIt);
    }
    return {};
  case Side::ASK:
    if (const auto foundIt = _asksHashTable.find(elem.price); foundIt != _asksHashTable.cend()) {
      foundIt->second->quantity = elem.quantity;
    } else {
      auto emplaceIt = _asks.emplace(_asks.cend(), elem);
      _asksHashTable.emplace(elem.price, emplaceIt);
    }
    return {};
  }

  return {};
}

std::error_code Book::del(double price) {
  if (!(std::fpclassify(price) == FP_NORMAL && price > 0)) {
    return std::make_error_code(std::errc::invalid_argument);
  }


  if (auto it = _asksHashTable.find(price); it != _asksHashTable.cend()) {
    if ((it->second + 1) != _asks.cend()) {
      auto place = it->second - _asks.begin();
      auto swappedIt = _asks.end() - 1;
      auto swappedPrice = swappedIt->price;
      std::iter_swap(it->second, swappedIt);
      _asksHashTable[swappedPrice] = _asks.begin() + place;
    }
    _asks.pop_back();
  }

  if (auto it = _bidsHashTable.find(price); it != _bidsHashTable.cend()) {
    if ((it->second + 1) != _bids.cend()) {
      auto place = it->second - _bids.begin();
      auto swappedIt = _bids.end() - 1;
      auto swappedPrice = swappedIt->price;
      std::iter_swap(it->second, swappedIt);
      _bidsHashTable[swappedPrice] = _asks.begin() + place;
    }
    _asks.pop_back();
  }

  return {};
}

double Book::vwap(size_t depth) {
  double sum = 0;
  double volumes = 0;
  std::vector<Element> orders(depth * 2);

  std::partial_sort_copy(_bids.cbegin(), _bids.cend(), orders.begin(),
                         orders.begin() + static_cast<std::vector<Element>::difference_type>(depth), LessElement{});
  std::partial_sort_copy(_asks.cbegin(), _asks.cend(),
                         orders.begin() + static_cast<std::vector<Element>::difference_type>(depth), orders.end(),
                         GreateElement{});

  for (const auto& elem : orders) {
    sum += elem.price * elem.quantity;
    volumes += elem.quantity;
  }

  return sum / volumes;
}

}  // namespace orderV3