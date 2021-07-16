#include <iostream>

#include "benchmark/benchmark.h"

#include "orderbook/OrderBook.h"


class AddOrderElementBenchmark: public benchmark::Fixture {
public:
  void SetUp(const ::benchmark::State& /*state*/) final { _book = order::Book{}; }

  void check(std::error_code ec) {
    if (ec != _empty_error_code) {
      throw std::logic_error(ec.message());
    }
  }

protected:
  order::Book _book;

private:
  static inline std::error_code _empty_error_code = {};
};

BENCHMARK_F(AddOrderElementBenchmark, AddLargerOrderElementAlways)(benchmark::State& state) {
  double startPrice = 1.01;
  double stepPrice = 0.01;
  double startQuantity = 1;
  double stepQuantity = 1;
  for (auto _ : state) {
    check(_book.add({startPrice, startQuantity, order::Side::BID}));
    check(_book.add({startPrice, startQuantity, order::Side::ASK}));
    startPrice += stepPrice;
    startQuantity += stepQuantity;
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations() * 2));
}

BENCHMARK_F(AddOrderElementBenchmark, AddSmallerOrderElementAlways)(benchmark::State& state) {
  order::Book book;
  double startPrice = 99999999.01;
  double stepPrice = 0.01;
  double startQuantity = 1;
  double stepQuantity = 1;
  for (auto _ : state) {
    check(book.add({startPrice, startQuantity, order::Side::BID}));
    check(book.add({startPrice, startQuantity, order::Side::ASK}));
    startPrice -= stepPrice;
    startQuantity += stepQuantity;
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations() * 2));
}

BENCHMARK_F(AddOrderElementBenchmark, AddMiddleOrderElementAlways)(benchmark::State& state) {
  order::Book book;
  double startPrice = 0.01;
  double endPrice = 99999999.01;
  double stepPrice = 0.01;
  double startQuantity = 1;
  double stepQuantity = 1;
  for (auto _ : state) {
    check(book.add({startPrice, startQuantity, order::Side::BID}));
    check(book.add({endPrice, startQuantity, order::Side::BID}));
    check(book.add({startPrice, startQuantity, order::Side::ASK}));
    check(book.add({endPrice, startQuantity, order::Side::ASK}));
    startPrice += stepPrice;
    endPrice -= stepPrice;
    startQuantity += stepQuantity;
  }

  state.SetItemsProcessed(static_cast<int64_t>(state.iterations() * 4));
}