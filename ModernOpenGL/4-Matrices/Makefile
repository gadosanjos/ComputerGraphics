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
all: $(BIN_DIR)/glm.exe $(BIN_DIR)/translation.exe $(BIN_DIR)/rotation.exe $(BIN_DIR)/scaling.exe

# Build target
$(BIN_DIR)/glm.exe: $(OBJ_DIR)/a.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/a.o -o $(BIN_DIR)/glm.exe $(LDFLAGS)

$(BIN_DIR)/translation.exe: $(OBJ_DIR)/translation.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/translation.o -o $(BIN_DIR)/translation.exe $(LDFLAGS)

$(BIN_DIR)/rotation.exe: $(OBJ_DIR)/rotation.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/rotation.o -o $(BIN_DIR)/rotation.exe $(LDFLAGS)

$(BIN_DIR)/scaling.exe: $(OBJ_DIR)/scaling.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/scaling.o -o $(BIN_DIR)/scaling.exe $(LDFLAGS)


# Compile source files into object files
$(OBJ_DIR)/a.o: $(SRC_DIR)/main.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/a.o

$(OBJ_DIR)/translation.o: $(SRC_DIR)/translation.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/translation.cpp -o $(OBJ_DIR)/translation.o

$(OBJ_DIR)/rotation.o: $(SRC_DIR)/rotation.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/rotation.cpp -o $(OBJ_DIR)/rotation.o

$(OBJ_DIR)/scaling.o: $(SRC_DIR)/scaling.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/scaling.cpp -o $(OBJ_DIR)/scaling.o

# Run the executable
run: all
	$(BIN_DIR)/glm.exe
	$(BIN_DIR)/translation.exe
	$(BIN_DIR)/rotation.exe
	$(BIN_DIR)/scaling.exe

# Clean up build artifacts
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/*.exe

.PHONY: clean