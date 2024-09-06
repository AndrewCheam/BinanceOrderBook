# Binance Order Book Project

This project implements a local order book for a given Binance trading symbol, such as `BTCUSDT`. It fetches an initial snapshot of the order book and then listens to real-time updates via WebSocket. The top 5 bid and ask levels are continuously updated and displayed in the console.

The purpose of this project is to demonstrate how to interact with Binance's API to maintain and display an order book in real-time.

## Setup and Installation

### Prerequisites

- Clang or any other C++ compiler
- `cURL` library for HTTP requests
- `WebSocket++`, `Boost`, `OpenSSL` library for WebSocket communication
- `nlohmann/json` for JSON parsing

### Installation

1. **Clone the Repository**:

   ```sh
   https://github.com/AndrewCheam/BinanceOrderBook.git
   cd binance_order_book
   ```

2. **Install Dependencies**:

   - Install `cURL`
   - Install `WebSocket++`
   - Install `nlohmann/json` (already downloaded as single include)

3. **Compile the Code**:
   ```sh
   make
   ```
4. **Run the Code**:
   ```sh
   .build/main
   ```

## Code Structure

### Files Overview

- `main.cpp`: The main entry point of the application. It handles user input, initializes the order book, and manages the WebSocket connection.
- `OrderBook.hpp` and `OrderBook.cpp`: Defines the `OrderBook` class, which manages the bid and ask levels.
- `websocket_client.hpp` and `websocket_client.cpp`: Defines the `WebSocketClient` class, which manages the connection to the websocket and notifies the orderbook upon any update.
- `http_client.cpp` and `http_client.hpp`: Defines http functions to call the initial snapshot

### Class and Function Descriptions

#### `OrderBook` Class

- **Purpose**: Manages the order book's bid and ask levels using two vectors.
- **Key Functions**:
  - `void printTopLevels(size_t n)`: Prints the top n bid and ask prices and quantity
  - `void processWebSocketMessage(nlohmann::json &message)`: When notified by the WebSocketClient, will update the Bid and Ask vector
  - `std::string fetchSnapshot()`: Fetches the initial snapshot of the bids and asks and stores it in a vector

#### `WebSocketClient` Class

- **Purpose**: Connects to the websocket stream
- **Key Functions**:
  - `void on_message(websocketpp::connection_hdl hdl, websocketpp::client<websocketpp::config::asio_tls_client>::message_ptr msg)` : Receives message payload from websocket connection and notifies orderbook to update the Bids and Asks.
  - `void updateAsk(double price, double quantity)`: Updates the ask price level.
  - `void printTopLevels()`: Prints the top 5 bid and ask levels.
