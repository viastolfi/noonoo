SRC_DIR     := src
BUILD_DIR   := build
INCLUDE_DIR := include

TARGET := $(BUILD_DIR)/noonoo

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))

UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

ifeq ($(UNAME_S),Darwin)
    PLATFORM := macos
    RAYLIB_ARCHIVE := raylib-5.5_macos.tar.gz
    RAYLIB_DIR := raylib-5.5_macos

    LDFLAGS := ./$(RAYLIB_DIR)/lib/libraylib.a \
               -framework CoreVideo \
               -framework IOKit \
               -framework Cocoa \
               -framework GLUT \
               -framework OpenGL \
               -framework AudioToolbox

else ifeq ($(UNAME_S),Linux)
    ifeq ($(UNAME_M),x86_64)
        PLATFORM := linux
        RAYLIB_ARCHIVE := raylib-5.5_linux_amd64.tar.gz
        RAYLIB_DIR := raylib-5.5_linux_amd64

        LDFLAGS := -L$(RAYLIB_DIR)/lib -l:libraylib.a -lm
    else
        $(error Unsupported architecture: $(UNAME_M))
    endif
else
    $(error Unsupported OS: $(UNAME_S))
endif

CXX := g++
CXXFLAGS := -Wall -Wextra -g -I$(INCLUDE_DIR) -I$(RAYLIB_DIR)/include

.PHONY: all clean check-raylib

all: check-raylib $(TARGET)

$(TARGET): $(OBJ)
	@echo "Linking..."
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

check-raylib:
	@if [ -d "$(RAYLIB_DIR)" ]; then \
		echo "raylib already installed"; \
	else \
		echo "Installing raylib for $(PLATFORM)..."; \
		mkdir $(RAYLIB_DIR); \
		wget https://github.com/raysan5/raylib/releases/download/5.5/$(RAYLIB_ARCHIVE); \
		tar -xzf $(RAYLIB_ARCHIVE) -C $(RAYLIB_DIR) --strip-components=1; \
		rm $(RAYLIB_ARCHIVE); \
		echo "raylib installed"; \
	fi

clean:
	rm -rf $(BUILD_DIR)
