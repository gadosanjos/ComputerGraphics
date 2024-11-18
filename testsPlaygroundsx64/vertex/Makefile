# Compiler
CXX := C:/msys64/mingw64/bin/g++.exe #runs in msys 64

# Directories
SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin
INCLUDE_DIR := ../Common/freeGLUT/include
LIB_DIR := ../Common/freeGLUT/lib
INCLUDE_DIR2 := ../Common/SOIL/include
LIB_DIR2 := ../Common/SOIL/lib
INCLUDE_DIR3 := ../Common/GLEW/include
LIB_DIR3 := ../Common/GLEW/lib/Release/x64
INCLUDE_DIR4 := ../Common/GLM

# Libraries to link against -lfreeglut -lSOIL -lopengl32 -lglu32 -luser32 -lgdi32 -lshell32 
LIBS := -lfreeglut -lSOIL -lglew32 -lopengl32 -luser32 -lgdi32 -lshell32

# Include directories
CXXFLAGS := -I$(INCLUDE_DIR) -I$(INCLUDE_DIR2) -I$(INCLUDE_DIR3) -I$(INCLUDE_DIR4)
LDFLAGS := -L$(LIB_DIR) -L$(LIB_DIR2) -L$(LIB_DIR3) $(LIBS)

#target: dependecies
#	action, must use a tab!not 4 spaces

# Output binary
all: $(BIN_DIR)/a.exe

# Build target
$(BIN_DIR)/a.exe: $(OBJ_DIR)/a.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/a.o -o $(BIN_DIR)/a.exe $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/a.o: $(SRC_DIR)/main.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/a.o

run: all
	$(BIN_DIR)/a.exe
clean: # calling make clean
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/a.exe

.PHONY: clean
# removes object files and tip and geom
# careful to have no space after * or will remove everything!