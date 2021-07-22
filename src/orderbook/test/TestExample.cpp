#include <cmath>

#include <gtest/gtest.h>

#include "orderbook/OrderBooks.h"


namespace {

template <typename OrderBookT>
class TestExample: public testing::Test {
protected:
  void setOrders(std::vector<order::Element>&& elements) {
    for (auto&& element : elements) {
      _book.add(element);
    }
  }

  void checkVwap(double expectedVwap, size_t depth, double digitals) {
    EXPECT_NEAR(expectedVwap, _book.vwap(depth), pow(0.1, digitals));
  }

  void checkOrderingBook() {
    auto orders = _book.orders();
    double maxBid = 0.;
    double minAsk = 0.;
    for (auto order : orders) {
      if (order.side == order::Side::BID) {
        if (maxBid == 0.) {
          maxBid = order.price;
        } else {
          EXPECT_GE(maxBid, order.price);
          maxBid = order.price;
        }
      }

      if (order.side == order::Side::ASK) {
        if (minAsk == 0.) {
          minAsk = order.price;
        } else {
          EXPECT_LE(minAsk, order.price);
          minAsk = order.price;
        }
      }
    }
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

TYPED_TEST_P(TestExample, CheckOrders) {
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

  this->checkOrderingBook();
}

TYPED_TEST_P(TestExample, OneBid) {
  this->setOrders({order::Element{37.3927, 11380, order::Side::BID}});

  this->checkVwap(37.3927, 5, 4.);
}

TYPED_TEST_P(TestExample, UniteOneBid) {
  this->setOrders({order::Element{37.3927, 11380, order::Side::BID}, order::Element{37.3927, 1, order::Side::BID}});

  EXPECT_TRUE(this->_book.orders().size() == 1);

  this->checkVwap(37.3927, 5, 4.);
}

TYPED_TEST_P(TestExample, TwoBidAndDelOne) {
  this->setOrders({order::Element{37.3927, 11380, order::Side::BID}, order::Element{37.3827, 11380, order::Side::BID}});

  this->_book.del(37.3927);

  this->checkVwap(37.3827, 5, 4.);
}

TYPED_TEST_P(TestExample, TwoBidAndDelAll) {
  this->setOrders({order::Element{37.3927, 11380, order::Side::BID}, order::Element{37.3827, 11380, order::Side::BID}});

  this->_book.del(37.3927);
  this->_book.del(37.3827);

  this->checkVwap(0., 5, 4.);
}

TYPED_TEST_P(TestExample, OneAsk) {
  this->setOrders({order::Element{37.3927, 11380, order::Side::ASK}});

  this->checkVwap(37.3927, 5, 4.);
}

TYPED_TEST_P(TestExample, UniteOneAsk) {
  this->setOrders({order::Element{37.3927, 11380, order::Side::ASK}, order::Element{37.3927, 1, order::Side::ASK}});

  EXPECT_TRUE(this->_book.orders().size() == 1);

  this->checkVwap(37.3927, 5, 4.);
}

TYPED_TEST_P(TestExample, TwoAskAndDelOne) {
  this->setOrders({order::Element{37.3927, 11380, order::Side::ASK}, order::Element{37.3827, 11380, order::Side::ASK}});

  this->_book.del(37.3927);

  this->checkVwap(37.3827, 5, 4.);
}

TYPED_TEST_P(TestExample, TwoAskAndDelAll) {
  this->setOrders({order::Element{37.3927, 11380, order::Side::ASK}, order::Element{37.3827, 11380, order::Side::ASK}});

  this->_book.del(37.3927);
  this->_book.del(37.3827);

  this->checkVwap(0., 5, 4.);
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

REGISTER_TYPED_TEST_SUITE_P(TestExample, RightExample, CheckOrders, OneBid, UniteOneBid, OneAsk, UniteOneAsk,
                            OneAskAndBid, TwoBidAndDelOne, TwoBidAndDelAll, TwoAskAndDelOne, TwoAskAndDelAll,
                            ManyAskAndOneBid, ManyBidAndOneAsk);

using Types = testing::Types<order::Book, orderV2::Book, orderV2_1::Book, orderV2_2::Book, orderV3::Book,
                             orderV3_1::Book, orderV3_2::Book>;
INSTANTIATE_TYPED_TEST_SUITE_P(test, TestExample, Types, );


}  // namespace