# Makefile

# Compiler to use
CXX = clang++

# Compiler flags
CXXFLAGS := -std=c++17 -Wall -Iinclude -Ilib/websocketpp -I/opt/homebrew/opt/boost/include -I/opt/homebrew/opt/openssl/include

# Linker flags
LDFLAGS := -lcurl -L/opt/homebrew/opt/boost/lib -L/opt/homebrew/opt/openssl/lib -lboost_system -lpthread -lssl -lcrypto

# The name of the output executable
TARGET = main

# Source files and object files
SRC_DIR = src
BUILD_DIR = build
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

# Default target: build the executable
all: $(BUILD_DIR)/$(TARGET)

# Rule to link object files into the final executable
$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# Rule to compile .cpp files into .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(BUILD_DIR)/$(TARGET)
	rm -f $(BUILD_DIR)/*.o
