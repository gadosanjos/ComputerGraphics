# Compiler
CXX := C:/msys64/mingw32/bin/g++.exe #runs in msys 32
# cd /c/Users/dosan/Desktop/Main/Fresno\ State/classes/14-Fall\ 2024/csci172/csci172_Projects/vscode/firstProject

# Directories
INCLUDE_DIR := ./Common/include
LIB_DIR := ./Common/lib

# Libraries to link against -lglut32 -lopengl32 -luser32 -lgdi32 -lshell32
LIBS := -lglut32 -lopengl32 -luser32 -lgdi32 -lshell32

# Include directories
CXXFLAGS := -I$(INCLUDE_DIR)
LDFLAGS := -L$(LIB_DIR) $(LIBS)

#target: dependecies
#	action, must use a tab!not 4 spaces

# Output binary
all: a.exe frame.exe

# Build target
a.exe: a.o
	$(CXX) a.o -o a.exe $(LDFLAGS)

# Compile source files into object files
a.o: main.cpp 
	$(CXX) $(CXXFLAGS) -c main.cpp -o a.o

# Framework target
frame.exe: frame.o
	$(CXX) frame.o -o frame.exe $(LDFLAGS)

frame.o: framework.cpp
	$(CXX) $(CXXFLAGS) -c framework.cpp -o frame.o

clean: #calling make clean
	rm -f *.o a

.PHONY: clean
# removes object files and tip and geom
# careful to have no space after * or will remove everything!