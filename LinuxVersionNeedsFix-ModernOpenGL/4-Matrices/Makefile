CXX := g++

# Directories
SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin

# Compiler / linker flags (GLEW, GLFW via pkg-config; GLM has only headers)
CXXFLAGS := $(shell pkg-config --cflags glew glfw3) -I/usr/include/glm
LDFLAGS  := $(shell pkg-config --libs glew glfw3)

# Output binaries
BINARIES := $(BIN_DIR)/glm $(BIN_DIR)/translation $(BIN_DIR)/rotation $(BIN_DIR)/scaling

all: $(BINARIES)

# Build targets
$(BIN_DIR)/glm: $(OBJ_DIR)/main.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/translation: $(OBJ_DIR)/translation.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/rotation: $(OBJ_DIR)/rotation.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/scaling: $(OBJ_DIR)/scaling.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/translation.o: $(SRC_DIR)/translation.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/rotation.o: $(SRC_DIR)/rotation.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/scaling.o: $(SRC_DIR)/scaling.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run all executables
run: all
	$(BIN_DIR)/glm
	$(BIN_DIR)/translation
	$(BIN_DIR)/rotation
	$(BIN_DIR)/scaling

# Clean up build artifacts
clean:
	rm -f $(OBJ_DIR)/*.o $(BINARIES)

.PHONY: clean run all
