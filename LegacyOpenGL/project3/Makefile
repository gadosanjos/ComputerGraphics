# Compiler selection based on the operating system
ifeq ($(OS),Windows_NT)
    # Windows-specific settings
    CXX := C:/msys64/mingw32/bin/g++.exe # 32-bit compiler for Windows
	LIBS := -lSOIL -lglut32 -lopengl32 -lglu32 -luser32 -lgdi32 -lshell32
	EXT := .exe
	INCLUDE_DIR := ../Common/include
	LIB_DIR := ../Common/lib
else
    # Linux (Ubuntu) settings
    CXX := g++ # Use the default system g++
	LIBS := -lSOIL -lglut -lGL -lGLU -lm
	EXT := # No extension needed for Linux binaries
	INCLUDE_DIR := /usr/include  # Use Ubuntu system includes
	LIB_DIR := /usr/lib  # Use Ubuntu system libraries
endif

# Directories
SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin

ifeq ($(OS),Windows_NT)
	CXXFLAGS := -I$(INCLUDE_DIR)
	LDFLAGS := -L$(LIB_DIR) $(LIBS)
else
# Include directories and 32-bit mode
	CXXFLAGS := -m32 -I$(INCLUDE_DIR)
	LDFLAGS := -m32 -L$(LIB_DIR) $(LIBS)
endif

# Output binary
all: $(BIN_DIR)/ModelTransformation$(EXT)

# Build target
$(BIN_DIR)/ModelTransformation$(EXT): $(OBJ_DIR)/a.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/a.o -o $(BIN_DIR)/ModelTransformation$(EXT) $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/a.o: $(SRC_DIR)/main.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/a.o

# Run the executable (detect if Windows or Linux)
run: all
ifeq ($(OS),Windows_NT)
	cd $(BIN_DIR) && ./ModelTransformation$(EXT)
else
	cd $(BIN_DIR) && ./ModelTransformation$(EXT)
endif

# Clean up build artifacts
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/*$(EXT)

.PHONY: clean
# careful to have no space after * or will remove everything!
