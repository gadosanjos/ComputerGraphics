CXX := g++

SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin

CXXFLAGS := $(shell pkg-config --cflags glew glfw3)
LDFLAGS  := $(shell pkg-config --libs glew glfw3)

all: $(BIN_DIR)/TestWindow

$(BIN_DIR)/TestWindow: $(OBJ_DIR)/main.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/main.o -o $(BIN_DIR)/TestWindow $(LDFLAGS)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/main.o

run: all
	$(BIN_DIR)/TestWindow

clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/TestWindow

.PHONY: clean run
