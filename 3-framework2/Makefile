# Compiler
CXX := C:/msys64/mingw32/bin/g++.exe #runs in msys 32

# Directories
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
all: Teapot.exe frame2.exe

# Build target
Teapot.exe: a.o
	$(CXX) a.o -o Teapot.exe $(LDFLAGS)

# Compile source files into object files
a.o: main.cpp 
	$(CXX) $(CXXFLAGS) -c main.cpp -o a.o

# Framework target
frame2.exe: frame.o
	$(CXX) frame.o -o frame.exe $(LDFLAGS)

frame.o: framework.cpp
	$(CXX) $(CXXFLAGS) -c framework.cpp -o frame.o

clean: #calling make clean
	rm -f *.o *.exe

.PHONY: clean
# removes object files and tip and geom
# careful to have no space after * or will remove everything!