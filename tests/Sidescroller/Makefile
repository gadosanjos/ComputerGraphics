# Compiler
CXX := C:/msys64/mingw32/bin/g++.exe # 32-bit

# Directories
SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin
INCLUDE_DIR1 := "../Common/GLEW/include"
INCLUDE_DIR2 := "../Common/GLFW/include"
INCLUDE_DIR3 := "../Common/GLM"
INCLUDE_DIR4 := ../Common/SOIL/include
LIB_DIR1 := "../Common/GLEW/lib/Release/Win32"
LIB_DIR2 := "../Common/GLFW/lib-mingw-w64"
LIB_DIR3 := ../Common/SOIL/lib

# Libraries to link against
LIBS := -lSOIL -lopengl32 -lglew32 -lglfw3 -lglu32 -luser32 -lgdi32 -lshell32

# Compiler flags
CXXFLAGS := -I$(INCLUDE_DIR1) -I$(INCLUDE_DIR2) -I$(INCLUDE_DIR3) -I$(INCLUDE_DIR4) -std=c++17
LDFLAGS := -L$(LIB_DIR1) -L$(LIB_DIR2) -L$(LIB_DIR3) $(LIBS)

# Output binary
TARGET := $(BIN_DIR)/main.exe

# Source and object filesp
SRCS := $(SRC_DIR)/main.cpp $(SRC_DIR)/Shader.cpp $(SRC_DIR)/Mesh.cpp $(SRC_DIR)/Window.cpp $(SRC_DIR)/Camera.cpp $(SRC_DIR)/Player.cpp
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
	cd $(BIN_DIR) && ./main.exe

# Clean up build artifacts
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

.PHONY: clean
