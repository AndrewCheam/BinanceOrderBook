#pragma once

#include <string>
#include <vector>
#include "websocket_client.hpp"
#include "json.hpp"

struct PriceQuantity
{
    std::string price;
    std::string quantity;
};

class OrderBook
{
public:
    OrderBook(const std::string &symbol);
    void start();
    void printTopLevels(size_t n);
    void processWebSocketMessage(nlohmann::json &message);
    void connectWebSocket();
    std::string fetchSnapshot();

private:
    WebSocketClient m_webSocketClient; // WebSocketClient instance

    std::string symbol;
    std::string url;
    std::string stream;
    long lastUpdateId;

    std::vector<PriceQuantity> topBids;
    std::vector<PriceQuantity> topAsks;
};