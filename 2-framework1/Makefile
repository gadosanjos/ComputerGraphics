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

# Output binaries
all: $(BIN_DIR)/BfractalFern.exe $(BIN_DIR)/frame.exe

# Build BfractalFern.exe
$(BIN_DIR)/BfractalFern.exe: $(OBJ_DIR)/a.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/a.o -o $(BIN_DIR)/BfractalFern.exe $(LDFLAGS)

# Compile a.o from main.cpp
$(OBJ_DIR)/a.o: $(SRC_DIR)/main.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/a.o

# Build frame.exe
$(BIN_DIR)/frame.exe: $(OBJ_DIR)/frame.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/frame.o -o $(BIN_DIR)/frame.exe $(LDFLAGS)

# Compile frame.o from framework.cpp
$(OBJ_DIR)/frame.o: $(SRC_DIR)/framework.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/framework.cpp -o $(OBJ_DIR)/frame.o

# Clean up build artifacts
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/*.exe

.PHONY: all clean
# removes object files and executables
# careful to have no space after * or will remove everything!