CXX := C:/msys64/mingw32/bin/g++.exe # 32 bit

# Directories
SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin
INCLUDE_DIR1 := "../External Libs/GLEW/include"
INCLUDE_DIR2 := "../External Libs/GLFW/include"
INCLUDE_DIR3 := "../External Libs/GLM"
LIB_DIR1 := "../External Libs/GLEW/lib/Release/Win32"
LIB_DIR2 := "../External Libs/GLFW/lib-mingw-w64"

# Libraries to link against-lopengl32 -lglew32 -lglfw3 -luser32 -lgdi32 -lshell32
LIBS := -lopengl32 -lglew32 -lglfw3 -lglu32 -luser32 -lgdi32 -lshell32

# Include directories
CXXFLAGS := -I$(INCLUDE_DIR1) -I$(INCLUDE_DIR2)  -I$(INCLUDE_DIR3)
LDFLAGS := -L$(LIB_DIR1) -L$(LIB_DIR2) $(LIBS)

# Output binary
all: $(BIN_DIR)/interpolation.exe $(BIN_DIR)/ibo_or_ebo.exe $(BIN_DIR)/projection.exe

# Build target
$(BIN_DIR)/interpolation.exe: $(OBJ_DIR)/a.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/a.o -o $(BIN_DIR)/interpolation.exe $(LDFLAGS)

$(BIN_DIR)/ibo_or_ebo.exe: $(OBJ_DIR)/ibo_or_ebo.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/ibo_or_ebo.o -o $(BIN_DIR)/ibo_or_ebo.exe $(LDFLAGS)

$(BIN_DIR)/projection.exe: $(OBJ_DIR)/projection.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/projection.o -o $(BIN_DIR)/projection.exe $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/a.o: $(SRC_DIR)/interpolation.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/interpolation.cpp -o $(OBJ_DIR)/a.o

$(OBJ_DIR)/ibo_or_ebo.o: $(SRC_DIR)/ibo_or_ebo.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/ibo_or_ebo.cpp -o $(OBJ_DIR)/ibo_or_ebo.o

$(OBJ_DIR)/projection.o: $(SRC_DIR)/projection.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/projection.cpp -o $(OBJ_DIR)/projection.o

# Run the executable
run: all
	$(BIN_DIR)/interpolation.exe
	$(BIN_DIR)/ibo_or_ebo.exe
	$(BIN_DIR)/projection.exe

# Clean up build artifacts
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/*.exe

.PHONY: clean