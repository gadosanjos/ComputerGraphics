CXX := g++

# Directories
SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin

# Compiler / linker flags
# (GLEW + GLFW via pkg-config; GLM is header-only, installed as libglm-dev)
CXXFLAGS := $(shell pkg-config --cflags glew glfw3) -I/usr/include/glm -std=c++17
LDFLAGS  := $(shell pkg-config --libs glew glfw3)

# Output binary
TARGET := $(BIN_DIR)/main

# Source and object files
SRCS := $(SRC_DIR)/main.cpp $(SRC_DIR)/Shader.cpp $(SRC_DIR)/Mesh.cpp $(SRC_DIR)/Window.cpp $(SRC_DIR)/Camera.cpp
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Build target
all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the executable
run: all
	cd $(BIN_DIR) && ./main

# Clean up build artifacts
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

.PHONY: clean run all
