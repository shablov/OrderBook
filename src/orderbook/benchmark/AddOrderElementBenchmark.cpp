#include <random>

#include "benchmark/benchmark.h"

#include "orderbook/OrderBooks.h"

static inline std::error_code _empty_error_code = {};
static bool check(std::error_code ec) {
  if (ec != _empty_error_code) {
    throw std::logic_error(ec.message());
  }
  return true;
}

template <typename OrderBookT>
void addLargerOrderElementAlways(benchmark::State& state) {
  OrderBookT _book;

  double price = 1.01;
  double stepPrice = 0.01;
  double quantity = 1;
  double stepQuantity = 1;
  for (auto _ : state) {
    check(_book.add({price, quantity, order::Side::BID}));
    check(_book.add({price, quantity, order::Side::ASK}));
    price += stepPrice;
    quantity += stepQuantity;
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations() * 2));
}

template <typename OrderBookT>
void addSmallerOrderElementAlways(benchmark::State& state) {
  OrderBookT _book;

  double price = 99999999.01;
  double stepPrice = 0.01;
  double quantity = 1;
  double stepQuantity = 1;
  for (auto _ : state) {
    check(_book.add({price, quantity, order::Side::BID}));
    check(_book.add({price, quantity, order::Side::ASK}));
    price -= stepPrice;
    quantity += stepQuantity;
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations() * 2));
}

template <typename OrderBookT>
void addMiddleOrderElementAlways(benchmark::State& state) {
  OrderBookT _book;

  double startPrice = 0.01;
  double endPrice = 99999999.01;
  double stepPrice = 0.01;
  double quantity = 1;
  double stepQuantity = 1;
  for (auto _ : state) {
    check(_book.add({startPrice, quantity, order::Side::BID}));
    check(_book.add({endPrice, quantity, order::Side::BID}));
    check(_book.add({startPrice, quantity, order::Side::ASK}));
    check(_book.add({endPrice, quantity, order::Side::ASK}));
    startPrice += stepPrice;
    endPrice -= stepPrice;
    quantity += stepQuantity;
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations() * 4));
}

BENCHMARK_TEMPLATE(addLargerOrderElementAlways, order::Book);
BENCHMARK_TEMPLATE(addLargerOrderElementAlways, orderV2::Book);
BENCHMARK_TEMPLATE(addLargerOrderElementAlways, orderV2_1::Book);
BENCHMARK_TEMPLATE(addLargerOrderElementAlways, orderV2_2::Book);
BENCHMARK_TEMPLATE(addLargerOrderElementAlways, orderV3::Book);
BENCHMARK_TEMPLATE(addLargerOrderElementAlways, orderV3_1::Book);
BENCHMARK_TEMPLATE(addLargerOrderElementAlways, orderEmpty::Book);

BENCHMARK_TEMPLATE(addSmallerOrderElementAlways, order::Book);
BENCHMARK_TEMPLATE(addSmallerOrderElementAlways, orderV2::Book);
BENCHMARK_TEMPLATE(addSmallerOrderElementAlways, orderV2_1::Book);
BENCHMARK_TEMPLATE(addSmallerOrderElementAlways, orderV2_2::Book);
BENCHMARK_TEMPLATE(addSmallerOrderElementAlways, orderV3::Book);
BENCHMARK_TEMPLATE(addSmallerOrderElementAlways, orderV3_1::Book);
BENCHMARK_TEMPLATE(addSmallerOrderElementAlways, orderEmpty::Book);

BENCHMARK_TEMPLATE(addMiddleOrderElementAlways, order::Book);
BENCHMARK_TEMPLATE(addMiddleOrderElementAlways, orderV2::Book);
BENCHMARK_TEMPLATE(addMiddleOrderElementAlways, orderV2_1::Book);
BENCHMARK_TEMPLATE(addMiddleOrderElementAlways, orderV2_2::Book);
BENCHMARK_TEMPLATE(addMiddleOrderElementAlways, orderV3::Book);
BENCHMARK_TEMPLATE(addMiddleOrderElementAlways, orderV3_1::Book);
BENCHMARK_TEMPLATE(addMiddleOrderElementAlways, orderEmpty::Book);
