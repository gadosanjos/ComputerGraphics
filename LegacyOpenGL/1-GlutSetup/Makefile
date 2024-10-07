# Compiler
CXX := C:/msys64/mingw32/bin/g++.exe #runs in msys 32

# Directories
SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin
INCLUDE_DIR := ../Common/include
LIB_DIR := ../Common/lib

# Libraries to link against -lglut32 -lopengl32 -luser32 -lgdi32 -lshell32
LIBS := -lglut32 -lopengl32 -luser32 -lgdi32 -lshell32

# Include directories
CXXFLAGS := -I$(INCLUDE_DIR)
LDFLAGS := -L$(LIB_DIR) $(LIBS)

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

clean: # calling make clean
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/a.exe

.PHONY: clean
# removes object files and tip and geom
# careful to have no space after * or will remove everything!