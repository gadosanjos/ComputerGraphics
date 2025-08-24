CXX := g++

# Directories
SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin

# Compiler / linker flags from pkg-config
CXXFLAGS := $(shell pkg-config --cflags glew glfw3)
LDFLAGS  := $(shell pkg-config --libs glew glfw3)

# Output binary
all: $(BIN_DIR)/FirstTriangle

# Build target
$(BIN_DIR)/FirstTriangle: $(OBJ_DIR)/main.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/main.o -o $(BIN_DIR)/FirstTriangle $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/main.o

# Run the executable
run: all
	$(BIN_DIR)/FirstTriangle

# Clean up build artifacts
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/FirstTriangle

.PHONY: clean run
