CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -pthread

SRC_DIR = src
OBJ_DIR = obj
TEST_DIR = tests
INCLUDE_DIR = include

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
TEST_CORE_OBJS = $(filter-out $(OBJ_DIR)/main.o, $(OBJECTS))

TARGET = webserver
TEST_TARGET = test_runner

# Default, builds the server
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

# builds test file from 'make test'
test: $(TEST_CORE_OBJS) $(TEST_DIR)/tests.cpp
	$(CXX) $(CXXFLAGS) $(TEST_CORE_OBJS) $(TEST_DIR)/tests.cpp -o $(TEST_TARGET)
	@echo "--------------------------"
	@echo "Running Unit Tests..."
	./$(TEST_TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(TEST_TARGET)

.PHONY: all clean test