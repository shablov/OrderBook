#include <random>

#include "benchmark/benchmark.h"

#include "orderbook/OrderBooks.h"

template <typename OrderBookT>
void addLargerOrderElementAlways(benchmark::State& state) {
  OrderBookT _book;

  double price = 1.01;
  double stepPrice = 0.01;
  double quantity = 1;
  double stepQuantity = 1;
  for (auto _ : state) {
    _book.add({price, quantity, order::Side::BID});
    _book.add({price, quantity, order::Side::ASK});
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
    _book.add({price, quantity, order::Side::BID});
    _book.add({price, quantity, order::Side::ASK});
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
    _book.add({startPrice, quantity, order::Side::BID});
    _book.add({endPrice, quantity, order::Side::BID});
    _book.add({startPrice, quantity, order::Side::ASK});
    _book.add({endPrice, quantity, order::Side::ASK});
    startPrice += stepPrice;
    endPrice -= stepPrice;
    quantity += stepQuantity;
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations() * 4));
}

template <typename OrderBookT>
void checkVwap(benchmark::State& state) {
  OrderBookT _book;

  double price = 1.01;
  double stepPrice = 0.01;
  double quantity = 1;
  double stepQuantity = 1;
  for (int i = 0; i < state.range(0); ++i) {
    _book.add({price, quantity, order::Side::BID});
    _book.add({price, quantity, order::Side::ASK});
    price += stepPrice;
    quantity += stepQuantity;
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(_book.vwap(20));
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations()));
}

BENCHMARK_TEMPLATE(addLargerOrderElementAlways, order::Book);
BENCHMARK_TEMPLATE(addLargerOrderElementAlways, orderV2::Book);
BENCHMARK_TEMPLATE(addLargerOrderElementAlways, orderV2_1::Book);
BENCHMARK_TEMPLATE(addLargerOrderElementAlways, orderV2_2::Book);
BENCHMARK_TEMPLATE(addLargerOrderElementAlways, orderV3::Book);
BENCHMARK_TEMPLATE(addLargerOrderElementAlways, orderV3_1::Book);
BENCHMARK_TEMPLATE(addLargerOrderElementAlways, orderV3_2::Book);
BENCHMARK_TEMPLATE(addLargerOrderElementAlways, orderEmpty::Book);

BENCHMARK_TEMPLATE(addSmallerOrderElementAlways, order::Book);
BENCHMARK_TEMPLATE(addSmallerOrderElementAlways, orderV2::Book);
BENCHMARK_TEMPLATE(addSmallerOrderElementAlways, orderV2_1::Book);
BENCHMARK_TEMPLATE(addSmallerOrderElementAlways, orderV2_2::Book);
BENCHMARK_TEMPLATE(addSmallerOrderElementAlways, orderV3::Book);
BENCHMARK_TEMPLATE(addSmallerOrderElementAlways, orderV3_1::Book);
BENCHMARK_TEMPLATE(addSmallerOrderElementAlways, orderV3_2::Book);
BENCHMARK_TEMPLATE(addSmallerOrderElementAlways, orderEmpty::Book);

BENCHMARK_TEMPLATE(addMiddleOrderElementAlways, order::Book);
BENCHMARK_TEMPLATE(addMiddleOrderElementAlways, orderV2::Book);
BENCHMARK_TEMPLATE(addMiddleOrderElementAlways, orderV2_1::Book);
BENCHMARK_TEMPLATE(addMiddleOrderElementAlways, orderV2_2::Book);
BENCHMARK_TEMPLATE(addMiddleOrderElementAlways, orderV3::Book);
BENCHMARK_TEMPLATE(addMiddleOrderElementAlways, orderV3_1::Book);
BENCHMARK_TEMPLATE(addMiddleOrderElementAlways, orderV3_2::Book);
BENCHMARK_TEMPLATE(addMiddleOrderElementAlways, orderEmpty::Book);


BENCHMARK_TEMPLATE(checkVwap, order::Book)->RangeMultiplier(16)->Range(1 << 8, 1 << 16)->ArgName("BookSize");
BENCHMARK_TEMPLATE(checkVwap, orderV2::Book)->RangeMultiplier(16)->Range(1 << 8, 1 << 16)->ArgName("BookSize");
BENCHMARK_TEMPLATE(checkVwap, orderV2_1::Book)->RangeMultiplier(16)->Range(1 << 8, 1 << 16)->ArgName("BookSize");
BENCHMARK_TEMPLATE(checkVwap, orderV2_2::Book)->RangeMultiplier(16)->Range(1 << 8, 1 << 16)->ArgName("BookSize");
BENCHMARK_TEMPLATE(checkVwap, orderV3::Book)->RangeMultiplier(16)->Range(1 << 8, 1 << 16)->ArgName("BookSize");
BENCHMARK_TEMPLATE(checkVwap, orderV3_1::Book)->RangeMultiplier(16)->Range(1 << 8, 1 << 16)->ArgName("BookSize");
BENCHMARK_TEMPLATE(checkVwap, orderV3_2::Book)->RangeMultiplier(16)->Range(1 << 8, 1 << 16)->ArgName("BookSize");
BENCHMARK_TEMPLATE(checkVwap, orderEmpty::Book)->RangeMultiplier(16)->Range(1 << 8, 1 << 16)->ArgName("BookSize");
