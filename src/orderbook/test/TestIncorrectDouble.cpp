#include <array>

#include <gtest/gtest.h>

#include "orderbook/OrderBook.h"
#include "orderbook/OrderBookV2.h"

namespace {

template<typename OrderBookT>
class IncorrectDoubleTest : public ::testing::Test {
public:
  void check(std::error_code ec) { EXPECT_EQ(std::make_error_code(std::errc::invalid_argument), ec); }

  OrderBookT _book;

protected:
  static inline std::array<double, 6> _range = {-1.0, 0.0, -0.0, 1.0 / 0.0, 0.0 / 0.0, DBL_MIN / 2};
};

TYPED_TEST_SUITE_P(IncorrectDoubleTest);

TYPED_TEST_P(IncorrectDoubleTest, AddIncorrectPrice) {
  for (auto value : IncorrectDoubleTest<TypeParam>::_range) {
    this->check(this->_book.add(order::Element{value, 11380, order::Side::BID}));
  }
}

TYPED_TEST_P(IncorrectDoubleTest, AddIncorrectQuantity) {
  for (auto value : IncorrectDoubleTest<TypeParam>::_range) {
    this->check(this->_book.add(order::Element{10.0, value, order::Side::ASK}));
  }
}

TYPED_TEST_P(IncorrectDoubleTest, ChangeIncorrectQuantity) {
  for (auto value : IncorrectDoubleTest<TypeParam>::_range) {
    this->check(this->_book.change(order::Element{10.0, value, order::Side::BID}));
  }
}

TYPED_TEST_P(IncorrectDoubleTest, ChangeIncorrectPrice) {
  for (auto value : IncorrectDoubleTest<TypeParam>::_range) {
    this->check(this->_book.change(order::Element{value, 10.0, order::Side::ASK}));
  }
}

TYPED_TEST_P(IncorrectDoubleTest, RemoveIncorrectPrice) {
  for (auto value : IncorrectDoubleTest<TypeParam>::_range) {
    this->check(this->_book.del(value));
  }
}

REGISTER_TYPED_TEST_SUITE_P(IncorrectDoubleTest, AddIncorrectPrice, AddIncorrectQuantity, ChangeIncorrectQuantity,
                            ChangeIncorrectPrice, RemoveIncorrectPrice);

using Types = testing::Types<order::Book, orderV2::Book>;
INSTANTIATE_TYPED_TEST_SUITE_P(test, IncorrectDoubleTest, Types,);

}