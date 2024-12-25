# Compiler
CXX := C:/msys64/mingw32/bin/g++.exe #runs in msys 32

# Directories
SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin
INCLUDE_DIR := ../Common/include
INCLUDE_DIR1 := ../Common/GLEW/include
LIB_DIR := ../Common/lib
LIB_DIR1 := ../Common/GLEW/lib/Release/Win32

# Libraries to link against -lglut32 -lopengl32 -luser32 -lgdi32 -lshell32, added -lSOIL -lglu32
LIBS := -lSOIL -lglew32 -lglut32 -lopengl32 -lglu32 -luser32 -lgdi32 -lshell32 

# undefined reference to `gluPerspective@32'are part of the GLU library, which is why -lglu32 is added
# undefined reference to `gluLookAt@72' are part of the GLU library, which is why -lglu32 is added

# Include directories
CXXFLAGS := -I$(INCLUDE_DIR) -I$(INCLUDE_DIR1)
LDFLAGS := -L$(LIB_DIR) -L$(LIB_DIR1) $(LIBS)

#target: dependecies
#	action, must use a tab!not 4 spaces

# Output binary
all: $(BIN_DIR)/frame3.exe

# Framework target
$(BIN_DIR)/frame3.exe: $(OBJ_DIR)/frame.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_DIR)/frame.o -o $(BIN_DIR)/frame3.exe $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/frame.o: $(SRC_DIR)/framework.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/framework.cpp -o $(OBJ_DIR)/frame.o

# Run ColorfullTeapot.exe
run: $(BIN_DIR)/frame3.exe
	cd $(BIN_DIR) && ./frame3.exe

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