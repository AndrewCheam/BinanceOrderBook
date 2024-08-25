#ifndef WEBSOCKET_CLIENT_HPP
#define WEBSOCKET_CLIENT_HPP

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
#include <string>
#include <memory>
#include "json.hpp"

class OrderBook; // Forward declaration
class WebSocketClient
{
public:
    WebSocketClient(OrderBook &orderBook);
    void connect(const std::string &uri);
    void send(const std::string &message);
    void close();

    std::vector<nlohmann::json> m_messages;

private:
    // Event handlers
    void on_open(websocketpp::connection_hdl hdl);
    void on_message(websocketpp::connection_hdl hdl, websocketpp::client<websocketpp::config::asio_tls_client>::message_ptr msg);
    void on_fail(websocketpp::connection_hdl hdl);
    void on_close(websocketpp::connection_hdl hdl);
    websocketpp::lib::shared_ptr<boost::asio::ssl::context> on_tls_init(const char *hostname, websocketpp::connection_hdl hdl);

    // Member variables
    websocketpp::client<websocketpp::config::asio_tls_client> m_client;
    websocketpp::connection_hdl m_hdl;
    OrderBook &m_orderBook;
};

#endif // WEBSOCKET_CLIENT_HPP
