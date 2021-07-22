#include "orderbook/OrderBookV3_2.h"

#include <cmath>

#include <range/v3/view/take.hpp>


namespace orderV3_2 {

void Book::add(Element elem) {
  /// Unite orders as NYSE exchange
  if (elem.side == Side::ASK) {
    if (const auto foundIt = _asksHashTable.find(elem.price); foundIt != _asksHashTable.cend()) {
      _asks[foundIt->second].quantity += elem.quantity;
    } else {
      auto emplaceIt = _asks.emplace(_asks.cend(), elem);
      _asksHashTable.emplace(elem.price, static_cast<uint64_t>(emplaceIt - _asks.begin()));
    }
  } else if (elem.side == Side::BID) {
    if (const auto foundIt = _bidsHashTable.find(elem.price); foundIt != _bidsHashTable.cend()) {
      _bids[foundIt->second].quantity += elem.quantity;
    } else {
      auto emplaceIt = _bids.emplace(_bids.cend(), elem);
      _bidsHashTable.emplace(elem.price, static_cast<uint64_t>(emplaceIt - _bids.begin()));
    }
  }
}

void Book::change(Element elem) {
  if (elem.side == Side::ASK) {
    if (const auto foundIt = _asksHashTable.find(elem.price); foundIt != _asksHashTable.cend()) {
      _asks[foundIt->second].quantity = elem.quantity;
    } else {
      auto emplaceIt = _asks.emplace(_asks.cend(), elem);
      _asksHashTable.emplace(elem.price, static_cast<uint64_t>(emplaceIt - _asks.begin()));
    }
  } else if (elem.side == Side::BID) {
    if (const auto foundIt = _bidsHashTable.find(elem.price); foundIt != _bidsHashTable.cend()) {
      _bids[foundIt->second].quantity = elem.quantity;
    } else {
      auto emplaceIt = _bids.emplace(_bids.cend(), elem);
      _bidsHashTable.emplace(elem.price, static_cast<uint64_t>(emplaceIt - _bids.begin()));
    }
  }
}

void Book::del(double price) {
  if (auto it = _asksHashTable.find(price); it != _asksHashTable.cend()) {
    if ((it->second + 1) != _asks.size()) {
      auto placeIt = _asks.begin() + static_cast<std::vector<Element>::difference_type>(it->second);
      auto swappedIt = _asks.end() - 1;
      auto swappedPrice = swappedIt->price;
      std::iter_swap(placeIt, swappedIt);
      _asksHashTable[swappedPrice] = it->second;
    }
    _asksHashTable.erase(it);
    _asks.pop_back();
  }

  if (auto it = _bidsHashTable.find(price); it != _bidsHashTable.cend()) {
    if ((it->second + 1) != _bids.size()) {
      auto placeIt = _bids.begin() + static_cast<std::vector<Element>::difference_type>(it->second);
      auto swappedIt = _bids.end() - 1;
      auto swappedPrice = swappedIt->price;
      std::iter_swap(placeIt, swappedIt);
      _bidsHashTable[swappedPrice] = it->second;
    }
    _bidsHashTable.erase(it);
    _bids.pop_back();
  }
}

double Book::vwap(size_t depth) {
  if (_bids.empty() && _asks.empty()) {
    return 0.;
  }

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

}  // namespace orderV3_2