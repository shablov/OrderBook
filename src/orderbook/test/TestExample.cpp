#include <cmath>

#include <gtest/gtest.h>

#include "orderbook/OrderBook.h"


namespace {

class TestExample: public testing::Test {
protected:
  void SetUp() final {}

  void TearDown() final {}

  void setOrders(std::vector<order::Element>&& elements) {
    for (auto&& element : elements) {
      _book.add(element);
    }
  }

  void checkVwap(double expectedVwap, size_t depth, size_t digitals) {
    EXPECT_NEAR(expectedVwap, _book.vwap(depth), pow(0.1, digitals));
  }

protected:
  order::Book _book;
};

TEST_F(TestExample, RightExample) {
  setOrders({order::Element{37.38, 11380, order::Side::BID},  order::Element{37.39, 19601, order::Side::BID},
             order::Element{37.40, 19760, order::Side::BID},  order::Element{37.41, 37236, order::Side::BID},
             order::Element{37.42, 65811, order::Side::BID},  order::Element{37.43, 367731, order::Side::BID},
             order::Element{37.44, 2056, order::Side::BID},   order::Element{37.45, 87281, order::Side::BID},
             order::Element{37.46, 72701, order::Side::BID},  order::Element{37.47, 49121, order::Side::BID},
             order::Element{37.28, 176828, order::Side::ASK}, order::Element{37.29, 142334, order::Side::ASK},
             order::Element{37.30, 155878, order::Side::ASK}, order::Element{37.31, 117585, order::Side::ASK},
             order::Element{37.32, 118253, order::Side::ASK}, order::Element{37.33, 209282, order::Side::ASK},
             order::Element{37.34, 385363, order::Side::ASK}, order::Element{37.35, 340823, order::Side::ASK},
             order::Element{37.36, 153408, order::Side::ASK}, order::Element{37.37, 187456, order::Side::ASK}});

  checkVwap(37.3543, 5, 4);
}

TEST_F(TestExample, OneBid) {
  setOrders({order::Element{37.3927, 11380, order::Side::BID}});

  checkVwap(37.3927, 5, 4);
}

TEST_F(TestExample, OneAsk) {
  setOrders({order::Element{37.3927, 11380, order::Side::ASK}});

  checkVwap(37.3927, 5, 4);
}

TEST_F(TestExample, OneAskAndBid) {
  setOrders({order::Element{37.40, 19760, order::Side::BID}, order::Element{37.38, 11380, order::Side::ASK}});

  checkVwap(37.3927, 5, 4);
}

TEST_F(TestExample, ManyAskAndOneBid) {
  setOrders({order::Element{37.47, 49121, order::Side::BID}, order::Element{37.28, 176828, order::Side::ASK},
             order::Element{37.29, 142334, order::Side::ASK}, order::Element{37.30, 155878, order::Side::ASK},
             order::Element{37.31, 117585, order::Side::ASK}, order::Element{37.32, 118253, order::Side::ASK},
             order::Element{37.33, 209282, order::Side::ASK}, order::Element{37.34, 385363, order::Side::ASK},
             order::Element{37.35, 340823, order::Side::ASK}, order::Element{37.36, 153408, order::Side::ASK},
             order::Element{37.37, 187456, order::Side::ASK}});

  checkVwap(37.3524, 5, 4);
}

TEST_F(TestExample, ManyBidAndOneAsk) {
  setOrders({order::Element{37.38, 11380, order::Side::BID}, order::Element{37.39, 19601, order::Side::BID},
             order::Element{37.40, 19760, order::Side::BID}, order::Element{37.41, 37236, order::Side::BID},
             order::Element{37.42, 65811, order::Side::BID}, order::Element{37.43, 367731, order::Side::BID},
             order::Element{37.44, 2056, order::Side::BID}, order::Element{37.45, 87281, order::Side::BID},
             order::Element{37.46, 72701, order::Side::BID}, order::Element{37.47, 49121, order::Side::BID},
             order::Element{37.28, 176828, order::Side::ASK}});

  checkVwap(37.3396, 5, 4);
}

}  // namespace