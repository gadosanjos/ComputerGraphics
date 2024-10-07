# Compiler
CXX := C:/msys64/mingw32/bin/g++.exe #runs in msys 32

# Directories
SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin
INCLUDE_DIR := ../Common/include
LIB_DIR := ../Common/lib

# Libraries to link against -lglut32 -lopengl32 -luser32 -lgdi32 -lshell32, added -lSOIL -lglu32
LIBS := -lSOIL -lglut32 -lopengl32 -lglu32 -luser32 -lgdi32 -lshell32 

# undefined reference to `gluPerspective@32'are part of the GLU library, which is why -lglu32 is added
# undefined reference to `gluLookAt@72' are part of the GLU library, which is why -lglu32 is added

# Include directories
CXXFLAGS := -I$(INCLUDE_DIR)
LDFLAGS := -L$(LIB_DIR) $(LIBS)

#target: dependecies
#	action, must use a tab!not 4 spaces

# Output binary
all: $(BIN_DIR)/ColorfullTeapot.exe $(BIN_DIR)/Teapot.exe $(BIN_DIR)/frame2.exe

# Build ColorfullTeapot.exe
$(BIN_DIR)/ColorfullTeapot.exe: $(OBJ_DIR)/tex.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/tex.o -o $(BIN_DIR)/ColorfullTeapot.exe $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/tex.o: $(SRC_DIR)/texturedTeapot.cpp 
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/texturedTeapot.cpp -o $(OBJ_DIR)/tex.o

# Build target
$(BIN_DIR)/Teapot.exe: $(OBJ_DIR)/a.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/a.o -o $(BIN_DIR)/Teapot.exe $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/a.o: $(SRC_DIR)/main.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/a.o

# Framework target
$(BIN_DIR)/frame2.exe: $(OBJ_DIR)/frame.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/frame.o -o $(BIN_DIR)/frame2.exe $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/frame.o: $(SRC_DIR)/framework.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/framework.cpp -o $(OBJ_DIR)/frame.o

# Run ColorfullTeapot.exe
run: $(BIN_DIR)/ColorfullTeapot.exe
	cd $(BIN_DIR) && ./ColorfullTeapot.exe

clean: #calling make clean
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/*.exe

.PHONY: clean
# removes object files and tip and geom
# careful to have no space after * or will remove everything!

# When you run the command make run from the root directory of your project, the working directory is where you executed the make run command. 
# In this case, the relative path ../images/t.jpg is being interpreted relative to the root directory, not the bin directory where your executable is located.

# However, when you navigate directly to the bin directory and run ./ColorfullTeapot.exe,
# the executable is looking for the image relative to the bin directory, which works as expected.

# Solution: cd $(BIN_DIR) && ./ColorfullTeapot.exe for run command