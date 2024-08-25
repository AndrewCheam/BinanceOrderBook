#include "websocket_client.hpp"
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
#include "json.hpp"
#include "order_book.hpp"

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

class OrderBook;
WebSocketClient::WebSocketClient(OrderBook &orderBook) : m_orderBook(orderBook)
{
    // Initialize ASIO
    m_client.init_asio();

    // Bind the handlers
    m_client.set_open_handler(websocketpp::lib::bind(&WebSocketClient::on_open, this, websocketpp::lib::placeholders::_1));
    m_client.set_message_handler(websocketpp::lib::bind(&WebSocketClient::on_message, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
    m_client.set_close_handler(websocketpp::lib::bind(&WebSocketClient::on_close, this, websocketpp::lib::placeholders::_1));
    m_client.set_fail_handler(websocketpp::lib::bind(&WebSocketClient::on_fail, this, websocketpp::lib::placeholders::_1));
    // need to readjust this part to be dynamic
    m_client.set_tls_init_handler(websocketpp::lib::bind(&WebSocketClient::on_tls_init, this, "stream.binance.com:9443/ws/bnbbtc@depth", websocketpp::lib::placeholders::_1));

    m_client.set_access_channels(websocketpp::log::alevel::all);
    m_client.clear_access_channels(websocketpp::log::alevel::frame_payload);
    m_client.set_error_channels(websocketpp::log::elevel::all);
}

void WebSocketClient::on_message(websocketpp::connection_hdl, websocketpp::client<websocketpp::config::asio_tls_client>::message_ptr msg)
{
    // std::cout << "Received message: " << msg->get_payload() << std::endl;
    nlohmann::json json_message = nlohmann::json::parse(msg->get_payload());
    m_messages.push_back(json_message);
    m_orderBook.processWebSocketMessage(json_message);
}

void WebSocketClient::connect(const std::string &uri)
{
    websocketpp::lib::error_code ec;
    auto con = m_client.get_connection(uri, ec);

    if (ec)
    {
        std::cout << "Could not create connection because: " << ec.message() << std::endl;
        return;
    }

    m_hdl = con->get_handle();
    m_client.connect(con);

    // Run the ASIO io_service in a separate thread
    std::thread([this]()
                { m_client.run(); })
        .detach();
}

void WebSocketClient::send(const std::string &message)
{
    websocketpp::lib::error_code ec;

    if (!m_hdl.expired())
    {
        m_client.send(m_hdl, message, websocketpp::frame::opcode::text, ec);

        if (ec)
        {
            std::cout << "Error sending message: " << ec.message() << std::endl;
        }
    }
    else
    {
        std::cout << "Connection handle is not valid." << std::endl;
    }
}

void WebSocketClient::close()
{
    websocketpp::lib::error_code ec;

    if (!m_hdl.expired())
    {
        m_client.close(m_hdl, websocketpp::close::status::normal, "", ec);

        if (ec)
        {
            std::cout << "Error closing connection: " << ec.message() << std::endl;
        }
    }
    else
    {
        std::cout << "Connection handle is not valid." << std::endl;
    }
}

void WebSocketClient::on_open(websocketpp::connection_hdl hdl)
{
    std::cout << "WebSocket connection opened!" << std::endl;
    m_hdl = hdl;
}

void WebSocketClient::on_fail(websocketpp::connection_hdl hdl)
{
    std::cerr << "WebSocket connection failed!" << std::endl;

    auto con = m_client.get_con_from_hdl(hdl);
    std::cerr << "Failure reason: " << con->get_ec().message() << std::endl;
}

void WebSocketClient::on_close(websocketpp::connection_hdl hdl)
{
    std::cout << "WebSocket connection closed!" << std::endl;
}

websocketpp::lib::shared_ptr<boost::asio::ssl::context> WebSocketClient::on_tls_init(const char *hostname, websocketpp::connection_hdl)
{
    context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
    try
    {
        ctx->set_options(boost::asio::ssl::context::default_workarounds |
                         boost::asio::ssl::context::no_sslv2 |
                         boost::asio::ssl::context::no_sslv3 |
                         boost::asio::ssl::context::single_dh_use);
    }
    catch (std::exception &e)
    {
        std::cout << "TLS Initialization Error: " << e.what() << std::endl;
    }

    return ctx;
}

// int main(int argc, char *argv[])
// {
//     WebSocketClient client;

//     std::string uri = "wss://stream.binance.com:9443/ws/bnbbtc@depth";
//     client.connect(uri);

//     std::string message;
//     while (true)
//     {
//         std::this_thread::sleep_for(std::chrono::milliseconds(100));
//     }

//     client.close();
//     std::this_thread::sleep_for(std::chrono::seconds(2));

//     return 0;
// }
