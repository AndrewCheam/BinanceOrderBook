#include "order_book.hpp"
#include "http_client.hpp"
#include "websocket_client.hpp"
#include "json.hpp"
#include <iostream>
#include <vector>
#include <iomanip>

using json = nlohmann::json;

OrderBook::OrderBook(const std::string &symbol, const size_t &numLevels) : m_webSocketClient(*this), symbol(symbol), numLevels(numLevels)
{
    url = "https://api.binance.com/api/v3/depth?symbol=" + symbol + "&limit=1000";
    std::string low_symbol = "";

    for (char ch : symbol)
    {
        // Convert each character to lowercase using tolower
        low_symbol += tolower(ch);
    }
    stream = "wss://stream.binance.com:9443/ws/" + low_symbol + "@depth";
}

void OrderBook::start()
{
    fetchSnapshot();
    connectWebSocket();
}

void OrderBook::connectWebSocket()
{

    m_webSocketClient.connect(stream);
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    m_webSocketClient.close();
}

std::string OrderBook::fetchSnapshot()
{
    std::string response = httpGet(url);
    json parsed_json = json::parse(response);

    // Clear previous data
    topBids.clear();
    topAsks.clear();

    // Process bids
    if (parsed_json.contains("bids"))
    {
        auto bids = parsed_json["bids"];

        for (auto &bid : bids)
        {
            PriceQuantity pq;
            pq.price = bid[0];    // Set the price attribute
            pq.quantity = bid[1]; // Set the quantity attribute

            // Add the object to the vector
            topBids.push_back(pq);
        }
    }

    // Process asks
    if (parsed_json.contains("asks"))
    {
        auto asks = parsed_json["asks"];
        for (auto &ask : asks)
        {
            PriceQuantity pq;
            pq.price = ask[0];    // Set the price attribute
            pq.quantity = ask[1]; // Set the quantity attribute

            // Add the object to the vector
            topAsks.push_back(pq);
        }
    }
    lastUpdateId = parsed_json["lastUpdateId"];
    std::cout << "THE VERY FIRST SNAPSHOT (first 20)" << std::endl;
    printTopLevels(numLevels);
    return response;
}

void OrderBook::processWebSocketMessage(json &message)
{
    // Drop any event where u is <= lastUpdateId
    if (message["u"] <= lastUpdateId || (message["U"] != lastUpdateId + 1 && m_webSocketClient.m_messages.size() != 1))
    {
        std::cout << "Dropped event: u <= lastUpdateId" << std::endl;
        std::cout << m_webSocketClient.m_messages.size() << std::endl;

        return;
    }
    for (const auto &bid : message["b"])
    {
        std::string price = bid[0];
        std::string quantity = bid[1];

        if (stoi(quantity) == 0)
        {
            // Remove the price level if quantity is 0
            auto it = std::remove_if(topBids.begin(), topBids.end(), [&price](const PriceQuantity &pq)
                                     { return pq.price == price; });
            topBids.erase(it, topBids.end());
        }
        else
        {
            // Update the price level
            auto it = std::find_if(topBids.begin(), topBids.end(), [&price](const PriceQuantity &pq)
                                   { return pq.price == price; });
            if (it != topBids.end())
            {
                it->quantity = quantity;
            }
            else
            {
                topBids.push_back({price, quantity});
            }
        }
    }

    // Update local order book for asks
    for (const auto &ask : message["a"])
    {
        std::string price = ask[0];
        std::string quantity = ask[1];

        if (stoi(quantity) == 0)
        {
            // Remove the price level if quantity is 0
            auto it = std::remove_if(topAsks.begin(), topAsks.end(), [&price](const PriceQuantity &pq)
                                     { return pq.price == price; });
            topAsks.erase(it, topAsks.end());
        }
        else
        {
            // Update the price level
            auto it = std::find_if(topAsks.begin(), topAsks.end(), [&price](const PriceQuantity &pq)
                                   { return pq.price == price; });
            if (it != topAsks.end())
            {
                it->quantity = quantity;
            }
            else
            {
                topAsks.push_back({price, quantity});
            }
        }
    }

    // Update lastUpdateId to the latest value
    lastUpdateId = message["u"];
    printTopLevels(numLevels);
}

void OrderBook::printTopLevels(size_t n)
{
    const int width = 15;
    const std::string separator = "+---------------+---------------+";
    const std::string header = "|     Price     |   Quantity    |";

    std::cout << separator << "\n";
    std::cout << header << "\n";
    std::cout << separator << "\n";

    // Print the top n bids
    std::cout << "Top Bids:\n";
    size_t count = 0;
    for (const auto &bid : topBids)
    {
        if (count++ >= n)
            break; // Stop after printing n items
        std::cout << "| " << std::setw(width) << std::left << bid.price
                  << "| " << std::setw(width) << std::left << bid.quantity << "|\n";
    }

    std::cout << separator << "\n";

    // Print the top n asks
    std::cout << "Top Asks:\n";
    count = 0;
    for (const auto &ask : topAsks)
    {
        if (count++ >= n)
            break; // Stop after printing n items
        std::cout << "| " << std::setw(width) << std::left << ask.price
                  << "| " << std::setw(width) << std::left << ask.quantity << "|\n";
    }

    std::cout << separator << "\n";
}
