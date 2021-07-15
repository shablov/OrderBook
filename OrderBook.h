#pragma once

enum class Side {
  BID,
  ASK
};

struct Element {
  double price;
  double quantity;
  Side side;
};

class OrderBook {
public:
  void add(Element elem);
  void change(Element elem);
  void del(double price);
  double vwap(size_t depth);
};
