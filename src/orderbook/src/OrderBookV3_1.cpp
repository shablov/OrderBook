#include "orderbook/OrderBookV3_1.h"

#include <cmath>

#include <range/v3/view/take.hpp>


namespace orderV3_1 {

std::error_code Book::add(Element elem) {
  if (!elem.is_valid()) {
    return std::make_error_code(std::errc::invalid_argument);
  }

  if (elem.side == Side::ASK) {
    bool needNormilize = _asks.capacity() == _asks.size();
    auto emplaceIt = _asks.emplace(_asks.cend(), elem);
    if (needNormilize) {
      for (auto it = _asks.begin(), end = _asks.end(); it != end; ++it) {
        _asksHashTable[it->price] = it;
      }
    } else {
      _asksHashTable.emplace(elem.price, emplaceIt);
    }
  } else if (elem.side == Side::BID) {
    bool needNormilize = _bids.capacity() == _bids.size();
    auto emplaceIt = _bids.emplace(_bids.cend(), elem);
    if (needNormilize) {
      for (auto it = _bids.begin(), end = _bids.end(); it != end; ++it) {
        _bidsHashTable[it->price] = it;
      }
    } else {
      _bidsHashTable.emplace(elem.price, emplaceIt);
    }
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
      bool needNormilize = _bids.capacity() == _bids.size();
      auto emplaceIt = _bids.emplace(_bids.cend(), elem);
      if (needNormilize) {
        for (auto it = _bids.begin(), end = _bids.end(); it != end; ++it) {
          _bidsHashTable[it->price] = it;
        }
      } else {
        _bidsHashTable.emplace(elem.price, emplaceIt);
      }
    }
    return {};
  case Side::ASK:
    if (const auto foundIt = _asksHashTable.find(elem.price); foundIt != _asksHashTable.cend()) {
      foundIt->second->quantity = elem.quantity;
    } else {
      bool needNormilize = _asks.capacity() == _asks.size();
      auto emplaceIt = _asks.emplace(_asks.cend(), elem);
      if (needNormilize) {
        for (auto it = _asks.begin(), end = _asks.end(); it != end; ++it) {
          _asksHashTable[it->price] = it;
        }
      } else {
        _asksHashTable.emplace(elem.price, emplaceIt);
      }
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

}  // namespace orderV3_1