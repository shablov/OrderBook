#include <random>
#include <iostream>

#include "benchmark/benchmark.h"

#include "orderbook/OrderBook.h"
#include "orderbook/OrderBookV2.h"
#include "orderbook/OrderBookV3.h"


static inline std::error_code _empty_error_code = {};
static bool check(std::error_code ec) {
  if (ec != _empty_error_code) {
    throw std::logic_error(ec.message());
  }
  return true;
}

template<typename OrderBookT>
static OrderBookT createBook(uint64_t startPrice, uint64_t stepPrice, double delimeter, uint64_t bookSize) {
    OrderBookT book;

    uint64_t price = startPrice;
    uint64_t quantity = 1;
    uint64_t stepQuantity = 1;
    for (size_t i = 0; i < bookSize; ++i) {
      check(book.add({price / delimeter, static_cast<double>(quantity), order::Side::BID}));
      check(book.add({price / delimeter, static_cast<double>(quantity), order::Side::ASK}));

      price += stepPrice;
      quantity += stepQuantity;
    }

    return book;
  }

template<typename OrderBookT>
static void changeTopTenPercentPrices(benchmark::State& state) {
  uint64_t startPrice = 10;
  uint64_t stepPrice = 1;
  double delimeter = 10.;

  auto bookSize = static_cast<uint64_t>(state.range(0));
  auto book = createBook<OrderBookT>(startPrice, stepPrice, delimeter, bookSize);

  auto topTenPercentEndPrice = startPrice + (bookSize / 10) * stepPrice;

  auto price = startPrice;
  for (auto _ : state) {
    check(book.change({price / delimeter, 1., order::Side::BID}));
    check(book.change({price / delimeter, 1., order::Side::ASK}));
    price += stepPrice;
    if (price >= topTenPercentEndPrice) {
      price = startPrice;
    }
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations() * 2));
}

template<typename OrderBookT>
static void changeAfterTenPercentPrices(benchmark::State& state) {
  uint64_t startPrice = 10;
  uint64_t stepPrice = 1;
  double delimeter = 10.;

  auto bookSize = static_cast<uint64_t>(state.range(0));
  auto book = createBook<OrderBookT>(startPrice, stepPrice, delimeter, bookSize);

  auto endPrice = startPrice + bookSize * stepPrice;
  auto topTenPercentEndPrice = startPrice + (bookSize / 10) * stepPrice;

  auto price = topTenPercentEndPrice;
  for (auto _ : state) {
    check(book.change({price / delimeter, 1., order::Side::BID}));
    check(book.change({price / delimeter, 1., order::Side::ASK}));
    price += stepPrice;
    if (price >= endPrice) {
      price = topTenPercentEndPrice;
    }
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations() * 2));
}


BENCHMARK_TEMPLATE(changeTopTenPercentPrices, order::Book)->RangeMultiplier(16)->Range(1 << 8, 1 << 16)->ArgName("BookSize");
BENCHMARK_TEMPLATE(changeTopTenPercentPrices, orderV2::Book)->RangeMultiplier(16)->Range(1 << 8, 1 << 16)->ArgName("BookSize");
BENCHMARK_TEMPLATE(changeTopTenPercentPrices, orderV3::Book)->RangeMultiplier(16)->Range(1 << 8, 1 << 16)->ArgName("BookSize");

BENCHMARK_TEMPLATE(changeAfterTenPercentPrices, order::Book)->RangeMultiplier(16)->Range(1 << 8, 1 << 16)->ArgName("BookSize");
BENCHMARK_TEMPLATE(changeAfterTenPercentPrices, orderV2::Book)->RangeMultiplier(16)->Range(1 << 8, 1 << 16)->ArgName("BookSize");
BENCHMARK_TEMPLATE(changeAfterTenPercentPrices, orderV3::Book)->RangeMultiplier(16)->Range(1 << 8, 1 << 16)->ArgName("BookSize");
