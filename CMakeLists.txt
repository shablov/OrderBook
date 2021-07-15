cmake_minimum_required(VERSION 3.17)
project(order_book)

set(CMAKE_CXX_STANDARD 20)

add_executable(order_book src/main.cpp src/OrderBook.cpp)