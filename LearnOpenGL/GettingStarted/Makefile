# --- Toolchain (keep 32-bit consistent with your libs) ---
CXX := C:/msys64/mingw32/bin/g++.exe
CC  := C:/msys64/mingw32/bin/gcc.exe

# --- Dirs ---
SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin

# --- Include/Lib dirs ---
INCLUDE_DIR1 := ../External Libs/GLFW/include
INCLUDE_DIR2 := ../External Libs/glad/include
LIB_DIR1     := ../External Libs/GLFW/lib-mingw-w64

# --- Flags ---
CXXFLAGS := -I"$(INCLUDE_DIR1)" -I"$(INCLUDE_DIR2)" -DGLFW_INCLUDE_NONE
CFLAGS   := $(CXXFLAGS)
LDFLAGS  := -L"$(LIB_DIR1)" -lglfw3 -lopengl32 -lgdi32 -luser32 -lshell32

# --- Auto-discover demos: each src/*.cpp => bin/<name>.exe ---
SRCS_MAIN    := $(notdir $(wildcard $(SRC_DIR)/*.cpp))
EXES         := $(patsubst %.cpp,$(BIN_DIR)/%.exe,$(SRCS_MAIN))
OBJS_MAIN    := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS_MAIN))

# --- Common objs shared by all demos ---
COMMON_OBJS  := $(OBJ_DIR)/glad.o
# If you add shared helpers (e.g., utils.cpp), list them here too:
# COMMON_OBJS += $(OBJ_DIR)/utils.o

# --- Default: build all discovered demos ---
all: $(EXES)

# --- Link rule: one exe per corresponding object + common objs ---
$(BIN_DIR)/%.exe: $(OBJ_DIR)/%.o $(COMMON_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS)

# --- Objects for each demo source ---
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- Common objects ---
$(OBJ_DIR)/glad.o: $(SRC_DIR)/glad.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Example for shared helpers if you add them:
# $(OBJ_DIR)/utils.o: $(SRC_DIR)/utils.cpp
# 	@mkdir -p $(OBJ_DIR)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- Run a specific demo: `make run PROGRAM=HelloTriangle` ---
run: $(BIN_DIR)/$(PROGRAM).exe
	$<

# --- Utilities ---
list:
	@echo "Demos found:" $(patsubst $(BIN_DIR)/%.exe,%,$(EXES))

clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/*.exe

.PHONY: all run clean list

# Easy run: make run PROGRAM=HelloTriangle (or TestWindow, etc.).
# make list