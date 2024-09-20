CXX := C:/msys64/mingw32/bin/g++.exe # 32 bit

# Directories
INCLUDE_DIR := ../Common/include
LIB_DIR := ../Common/lib

# Libraries to link against -lglut32 -lopengl32 -luser32 -lgdi32 -lshell32
LIBS := -lglut32 -lopengl32 -luser32 -lgdi32 -lshell32

# Include directories
CXXFLAGS := -I$(INCLUDE_DIR)
LDFLAGS := -L$(LIB_DIR) $(LIBS)

# Output binary
all: BfractalFern.exe

# Build target
BfractalFern.exe: a.o
	$(CXX) a.o -o BfractalFern.exe $(LDFLAGS)

# Compile source files into object files
a.o: main.cpp 
	$(CXX) $(CXXFLAGS) -c main.cpp -o a.o

clean: #calling make clean
	rm -f *.o *.exe

.PHONY: clean
# careful to have no space after * or will remove everything!
