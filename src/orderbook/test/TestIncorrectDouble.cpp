#include <array>

#include <gtest/gtest.h>

#include "orderbook/OrderBooks.h"

namespace {

template <typename OrderBookT>
class IncorrectDoubleTest: public ::testing::Test {
public:
  void check(std::error_code ec) { EXPECT_EQ(std::make_error_code(std::errc::invalid_argument), ec); }

  OrderBookT _book;

protected:
  static inline std::array<double, 6> _range = {-1.0, 0.0, -0.0, 1.0 / 0.0, 0.0 / 0.0, DBL_MIN / 2};
};

TYPED_TEST_SUITE_P(IncorrectDoubleTest);

TYPED_TEST_P(IncorrectDoubleTest, AddIncorrectPrice) {
  GTEST_SKIP_("Removed error code for check input data");
  //  for (auto value : IncorrectDoubleTest<TypeParam>::_range) {
  //    this->check(this->_book.add(order::Element{value, 11380, order::Side::BID}));
  //  }
}

TYPED_TEST_P(IncorrectDoubleTest, AddIncorrectQuantity) {
  GTEST_SKIP_("Removed error code for check input data");
  //  for (auto value : IncorrectDoubleTest<TypeParam>::_range) {
  //    this->check(this->_book.add(order::Element{10.0, value, order::Side::ASK}));
  //  }
}

TYPED_TEST_P(IncorrectDoubleTest, ChangeIncorrectQuantity) {
  GTEST_SKIP_("Removed error code for check input data");
  //  for (auto value : IncorrectDoubleTest<TypeParam>::_range) {
  //    this->check(this->_book.change(order::Element{10.0, value, order::Side::BID}));
  //  }
}

TYPED_TEST_P(IncorrectDoubleTest, ChangeIncorrectPrice) {
  GTEST_SKIP_("Removed error code for check input data");
  //  for (auto value : IncorrectDoubleTest<TypeParam>::_range) {
  //    this->check(this->_book.change(order::Element{value, 10.0, order::Side::ASK}));
  //  }
}

TYPED_TEST_P(IncorrectDoubleTest, RemoveIncorrectPrice) {
  GTEST_SKIP_("Removed error code for check input data");
  //  for (auto value : IncorrectDoubleTest<TypeParam>::_range) {
  //    this->check(this->_book.del(value));
  //  }
}

REGISTER_TYPED_TEST_SUITE_P(IncorrectDoubleTest, AddIncorrectPrice, AddIncorrectQuantity, ChangeIncorrectQuantity,
                            ChangeIncorrectPrice, RemoveIncorrectPrice);

using Types =
    testing::Types<order::Book, orderV2::Book, orderV2_1::Book, orderV2_2::Book, orderV3::Book, orderV3_1::Book>;
INSTANTIATE_TYPED_TEST_SUITE_P(test, IncorrectDoubleTest, Types, );

}  // namespace