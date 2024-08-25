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

   - Install `cURL`:
   - Install `WebSocket++`:
   - Install `nlohmann/json` (already downloaded as single include)

3. **Compile the Code**:
   ```sh
   make
   ```
4. **Run the Code**:
   ```sh
   .build/main
   ```
