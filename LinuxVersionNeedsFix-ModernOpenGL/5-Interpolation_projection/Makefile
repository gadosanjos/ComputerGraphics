CXX := g++

# Directories
SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin

# Compiler / linker flags (GLEW, GLFW via pkg-config; GLM header-only)
CXXFLAGS := $(shell pkg-config --cflags glew glfw3) -I/usr/include/glm
LDFLAGS  := $(shell pkg-config --libs glew glfw3)

# Output binaries
BINARIES := $(BIN_DIR)/interpolation $(BIN_DIR)/ibo_or_ebo $(BIN_DIR)/projection

all: $(BINARIES)

# Build targets
$(BIN_DIR)/interpolation: $(OBJ_DIR)/interpolation.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/ibo_or_ebo: $(OBJ_DIR)/ibo_or_ebo.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/projection: $(OBJ_DIR)/projection.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/interpolation.o: $(SRC_DIR)/interpolation.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/ibo_or_ebo.o: $(SRC_DIR)/ibo_or_ebo.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/projection.o: $(SRC_DIR)/projection.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run all executables
run: all
	$(BIN_DIR)/interpolation
	$(BIN_DIR)/ibo_or_ebo
	$(BIN_DIR)/projection

# Clean up build artifacts
clean:
	rm -f $(OBJ_DIR)/*.o $(BINARIES)

.PHONY: clean run all
