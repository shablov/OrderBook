#include <gtest/gtest.h>

#include "orderbook/OrderBook.h"


class IncorrectDoubleTest: public testing::TestWithParam<double> {
public:
  void check(std::error_code ec) { EXPECT_EQ(std::make_error_code(std::errc::invalid_argument), ec); }

protected:
  order::Book _book;
};

TEST_P(IncorrectDoubleTest, AddIncorrectPrice) {
  check(_book.add(order::Element{GetParam(), 11380, order::Side::BID}));
}

TEST_P(IncorrectDoubleTest, AddIncorrectQuantity) {
  check(_book.add(order::Element{10.0, GetParam(), order::Side::ASK}));
}

TEST_P(IncorrectDoubleTest, ChangeIncorrectQuantity) {
  check(_book.change(order::Element{10.0, GetParam(), order::Side::BID}));
}

TEST_P(IncorrectDoubleTest, ChangeIncorrectPrice) {
  check(_book.change(order::Element{GetParam(), 10.0, order::Side::ASK}));
}

TEST_P(IncorrectDoubleTest, RemoveIncorrectPrice) {
  check(_book.del(GetParam()));
}

INSTANTIATE_TEST_SUITE_P(IncorrectDouble, IncorrectDoubleTest,
                         testing::Values(-1.0, 0.0, -0.0, 1.0 / 0.0, 0.0 / 0.0, DBL_MIN / 2));