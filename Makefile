CC = g++
CFLAGS = -I./include -g -Wall -DMEM_DBG

SRC_DIR = src
BUILD_DIR = build
TEST_DIR = test

SRC = $(wildcard $(SRC_DIR)/*.cpp)          
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC)) 
LIBRARY = $(BUILD_DIR)/libMemoryTracker.a

TEST_SRC = $(TEST_DIR)/Test1.cpp
TEST_EXEC = $(BUILD_DIR)/Test1

all: $(BUILD_DIR) $(LIBRARY) $(TEST_EXEC)

$(LIBRARY): $(OBJ)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_EXEC): $(TEST_SRC) $(LIBRARY)
	$(CC) $(CFLAGS) -o $@ $< -L$(BUILD_DIR) -lMemoryTracker

run: $(TEST_EXEC)
	./$(TEST_EXEC)

clean:
	rm -f $(OBJ) $(LIBRARY) $(TEST_EXEC)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: all run clean
