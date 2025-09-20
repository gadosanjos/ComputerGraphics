# Compiler
CXX := C:/msys64/mingw32/bin/g++.exe # 32-bit

# Directories
SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin
INCLUDE_DIR1 := "../Common/GLEW/include"
INCLUDE_DIR2 := "../Common/GLFW/include"
INCLUDE_DIR3 := "../Common/GLM"
INCLUDE_DIR4 := ../Common/SOIL/include
LIB_DIR1 := "../Common/GLEW/lib/Release/Win32"
LIB_DIR2 := "../Common/GLFW/lib-mingw-w64"
LIB_DIR3 := ../Common/SOIL/lib

# Libraries to link against
LIBS := -lSOIL -lopengl32 -lglew32 -lglfw3 -lglu32 -luser32 -lgdi32 -lshell32

# Compiler flags
CXXFLAGS := -I$(INCLUDE_DIR1) -I$(INCLUDE_DIR2) -I$(INCLUDE_DIR3) -I$(INCLUDE_DIR4) -std=c++17
LDFLAGS := -L$(LIB_DIR1) -L$(LIB_DIR2) -L$(LIB_DIR3) $(LIBS)

# Executables (two separate programs, like the CMake)
TARGET_2D := $(BIN_DIR)/BlackHole2D.exe
TARGET_3D := $(BIN_DIR)/BlackHole3D.exe

# Sources per program (each has its own main)
SRCS_2D := $(SRC_DIR)/2D_lensing.cpp
SRCS_3D := $(SRC_DIR)/black_hole.cpp

# Objects per program
OBJS_2D := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS_2D))
OBJS_3D := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS_3D))

# Shaders to copy (like CMake glob)
SHADERS := $(wildcard $(SRC_DIR)/*.vert) $(wildcard $(SRC_DIR)/*.frag) $(wildcard $(SRC_DIR)/*.comp)

# Build target
all: $(TARGET_2D) $(TARGET_3D)

# Link rules
$(TARGET_2D): $(OBJS_2D) | $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(TARGET_3D): $(OBJS_3D) | $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS)
# Copy shaders after 3D build (mirrors CMake's copy_if_different)
ifneq ($(strip $(SHADERS)),)
	@for f in $(SHADERS); do \
	  dest="$(BIN_DIR)/$$(basename $$f)"; \
	  if [ ! -f "$$dest" ] || ! cmp -s "$$f" "$$dest"; then \
	    cp -f "$$f" "$(BIN_DIR)/"; \
	    echo "Copied shader: $$f -> $(BIN_DIR)/"; \
	  fi; \
	done
endif

# Pattern rule: compile any .cpp under SRC_DIR to .o under OBJ_DIR
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure dirs exist
$(BIN_DIR) $(OBJ_DIR):
	@mkdir -p $@

# Convenience run targets
run2d: $(TARGET_2D)
	cd $(BIN_DIR) && ./$(notdir $(TARGET_2D))

run3d: $(TARGET_3D)
	cd $(BIN_DIR) && ./$(notdir $(TARGET_3D))

# Clean
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/BlackHole2D.exe $(BIN_DIR)/BlackHole3D.exe

.PHONY: all clean run2d run3d