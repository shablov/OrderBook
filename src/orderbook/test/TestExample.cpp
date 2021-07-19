#include <cmath>

#include <gtest/gtest.h>

#include "orderbook/OrderBooks.h"


namespace {

template <typename OrderBookT>
class TestExample: public testing::Test {
protected:
  void setOrders(std::vector<order::Element>&& elements) {
    for (auto&& element : elements) {
      auto ec = _book.add(element);
      EXPECT_EQ(ec, std::error_code{});
    }
  }

  void checkVwap(double expectedVwap, size_t depth, double digitals) {
    EXPECT_NEAR(expectedVwap, _book.vwap(depth), pow(0.1, digitals));
  }

protected:
  OrderBookT _book;
};

TYPED_TEST_SUITE_P(TestExample);

TYPED_TEST_P(TestExample, RightExample) {
  this->setOrders({order::Element{37.38, 11380, order::Side::BID},  order::Element{37.39, 19601, order::Side::BID},
                   order::Element{37.40, 19760, order::Side::BID},  order::Element{37.41, 37236, order::Side::BID},
                   order::Element{37.42, 65811, order::Side::BID},  order::Element{37.43, 367731, order::Side::BID},
                   order::Element{37.44, 2056, order::Side::BID},   order::Element{37.45, 87281, order::Side::BID},
                   order::Element{37.46, 72701, order::Side::BID},  order::Element{37.47, 49121, order::Side::BID},
                   order::Element{37.28, 176828, order::Side::ASK}, order::Element{37.29, 142334, order::Side::ASK},
                   order::Element{37.30, 155878, order::Side::ASK}, order::Element{37.31, 117585, order::Side::ASK},
                   order::Element{37.32, 118253, order::Side::ASK}, order::Element{37.33, 209282, order::Side::ASK},
                   order::Element{37.34, 385363, order::Side::ASK}, order::Element{37.35, 340823, order::Side::ASK},
                   order::Element{37.36, 153408, order::Side::ASK}, order::Element{37.37, 187456, order::Side::ASK}});

  this->checkVwap(37.3543, 5, 4.);
}

TYPED_TEST_P(TestExample, OneBid) {
  this->setOrders({order::Element{37.3927, 11380, order::Side::BID}});

  this->checkVwap(37.3927, 5, 4.);
}

TYPED_TEST_P(TestExample, OneAsk) {
  this->setOrders({order::Element{37.3927, 11380, order::Side::ASK}});

  this->checkVwap(37.3927, 5, 4.);
}

TYPED_TEST_P(TestExample, OneAskAndBid) {
  this->setOrders({order::Element{37.40, 19760, order::Side::BID}, order::Element{37.38, 11380, order::Side::ASK}});

  this->checkVwap(37.3927, 5, 4.);
}

TYPED_TEST_P(TestExample, ManyAskAndOneBid) {
  this->setOrders({order::Element{37.47, 49121, order::Side::BID}, order::Element{37.28, 176828, order::Side::ASK},
                   order::Element{37.29, 142334, order::Side::ASK}, order::Element{37.30, 155878, order::Side::ASK},
                   order::Element{37.31, 117585, order::Side::ASK}, order::Element{37.32, 118253, order::Side::ASK},
                   order::Element{37.33, 209282, order::Side::ASK}, order::Element{37.34, 385363, order::Side::ASK},
                   order::Element{37.35, 340823, order::Side::ASK}, order::Element{37.36, 153408, order::Side::ASK},
                   order::Element{37.37, 187456, order::Side::ASK}});

  this->checkVwap(37.3524, 5, 4.);
}

TYPED_TEST_P(TestExample, ManyBidAndOneAsk) {
  this->setOrders({order::Element{37.38, 11380, order::Side::BID}, order::Element{37.39, 19601, order::Side::BID},
                   order::Element{37.40, 19760, order::Side::BID}, order::Element{37.41, 37236, order::Side::BID},
                   order::Element{37.42, 65811, order::Side::BID}, order::Element{37.43, 367731, order::Side::BID},
                   order::Element{37.44, 2056, order::Side::BID}, order::Element{37.45, 87281, order::Side::BID},
                   order::Element{37.46, 72701, order::Side::BID}, order::Element{37.47, 49121, order::Side::BID},
                   order::Element{37.28, 176828, order::Side::ASK}});

  this->checkVwap(37.3396, 5, 4.);
}

REGISTER_TYPED_TEST_SUITE_P(TestExample, RightExample, OneBid, OneAsk, OneAskAndBid, ManyAskAndOneBid,
                            ManyBidAndOneAsk);

using Types =
    testing::Types<order::Book, orderV2::Book, orderV2_1::Book, orderV2_2::Book, orderV3::Book, orderV3_1::Book>;
INSTANTIATE_TYPED_TEST_SUITE_P(test, TestExample, Types, );


}  // namespace