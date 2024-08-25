#include "order_book.hpp"
// #include "websocket_client.hpp" // Adjust the include path if necessary
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

int main()
{
    std::string symbol;
    std::cout << "Enter Binance symbol (e.g., BTCUSDT): ";
    std::cin >> symbol;

    OrderBook orderBook(symbol);
    orderBook.start();

    return 0;
}
