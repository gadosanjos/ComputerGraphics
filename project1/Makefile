CXX := C:/msys64/mingw32/bin/g++.exe # 32 bit

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

# Output binary
all: $(BIN_DIR)/BfractalFern.exe 

# Build target
$(BIN_DIR)/BfractalFern.exe: $(OBJ_DIR)/a.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/a.o -o $(BIN_DIR)/BfractalFern.exe $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/a.o: $(SRC_DIR)/main.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/a.o

# Clean up build artifacts
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/*.exe

.PHONY: clean
# careful to have no space after * or will remove everything!
