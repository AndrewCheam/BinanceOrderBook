#include "order_book.hpp"
// #include "websocket_client.hpp" // Adjust the include path if necessary
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

int main()
{
    std::string symbol;
    size_t numLevels;
    std::cout << "Enter Binance symbol (e.g., BTCUSDT): ";
    std::cin >> symbol;
    std::cout << "Enter Number of levels to print: ";
    std::cin >> numLevels;

    OrderBook orderBook(symbol, numLevels);
    orderBook.start();

    return 0;
}
