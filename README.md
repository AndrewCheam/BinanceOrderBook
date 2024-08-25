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

### 5. **Code Structure**

- **Overview of Key Files**: vnewirjfnwr
- **Class and Function Documentation**: Provide descriptions for the main classes and functions, including their roles and any important implementation details.

**Example:**

```markdown
## Code Structure

### Files Overview

- `main.cpp`: The main entry point of the application. It handles user input, initializes the order book, and manages the WebSocket connection.
- `OrderBook.hpp` and `OrderBook.cpp`: Defines the `OrderBook` class, which manages the bid and ask levels.
- `utils.cpp`: Contains utility functions for handling HTTP requests and JSON parsing.

### Class and Function Descriptions

#### `OrderBook` Class

- **Purpose**: Manages the order book's bid and ask levels using two maps.
- **Key Functions**:
  - `void updateBid(double price, double quantity)`: Updates the bid price level.
  - `void updateAsk(double price, double quantity)`: Updates the ask price level.
  - `void printTopLevels()`: Prints the top 5 bid and ask levels.

#### `fetchSnapshot()` Function

- **Purpose**: Makes an HTTP GET request to Binance's API to retrieve the initial order book snapshot.
- **Parameters**: `string symbol` - The trading pair symbol (e.g., `BTCUSDT`).
- **Returns**: A `json` object containing the snapshot data.
```
