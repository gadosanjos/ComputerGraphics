CXX := C:/msys64/mingw32/bin/g++.exe # 32 bit

# Directories
SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin
INCLUDE_DIR1 := "../External Libs/GLEW/include"
INCLUDE_DIR2 := "../External Libs/GLFW/include"
LIB_DIR1 := "../External Libs/GLEW/lib/Release/Win32"
LIB_DIR2 := "../External Libs/GLFW/lib-mingw-w64"

# Libraries to link against-lopengl32 -lglew32 -lglfw3 -luser32 -lgdi32 -lshell32
LIBS := -lopengl32 -lglew32 -lglfw3 -lglu32 -luser32 -lgdi32 -lshell32

# Include directories
CXXFLAGS := -I$(INCLUDE_DIR1) -I$(INCLUDE_DIR2)
LDFLAGS := -L$(LIB_DIR1) -L$(LIB_DIR2) $(LIBS)

# Output binary
all: $(BIN_DIR)/FirstTriangle.exe 

# Build target
$(BIN_DIR)/FirstTriangle.exe: $(OBJ_DIR)/a.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/a.o -o $(BIN_DIR)/FirstTriangle.exe $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/a.o: $(SRC_DIR)/main.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/a.o

# Run the executable
run: all
	$(BIN_DIR)/FirstTriangle.exe

# Clean up build artifacts
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/*.exe

.PHONY: clean