SRC = src
BUILD = build
RAYLIB_LINUX_X86_64_LIB = raylib-5.5_linux_amd64
RAYLIB_DARWIN_LIB = raylib-5.5_macos
RAYLIB_FOLDER = raylib

CS = \
		 $(SRC)/main.cpp \
		 $(SRC)/Renderer.cpp \
		 $(SRC)/Game.cpp

OBJ = \
			$(BUILD)/main.o \
			$(BUILD)/Renderer.o \
			$(BUILD)/Game.o

CC = g++
CFLAGS = -Wall -Wextra -g

# Detect platform
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

.PHONY: all clean check-platform

all: check-platform $(BUILD)/noonoo

$(BUILD)/noonoo: $(OBJ)
	$(CC) -o $@ $^ -L$(RAYLIB_FOLDER)/lib -l:libraylib.a -lm

$(BUILD)/%.o: $(SRC)/%.cpp 
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(RAYLIB_FOLDER)/include -Iinclude

check-platform:
	@sh -c '\
		if [ "$(UNAME_S)" == "Darwin" ]; then \
			if [ -d "$(RAYLIB_FOLDER)" ]; then \
				echo "raylib already installed for your lib"; \
				exit 0; \
			else \
				echo "Installing Raylib library for MacOS..."; \
				mkdir $(RAYLIB_FOLDER); \
				wget https://github.com/raysan5/raylib/releases/download/5.5/$(RAYLIB_DARWIN_LIB).tar.gz; \
				tar -xzf $(RAYLIB_DARWIN_LIB).tar.gz -C $(RAYLIB_FOLDER) --strip-component 1; \
				rm $(RAYLIB_DARWIN_LIB).tar.gz; \
				echo "raylib downloaded and extracted"; \
			fi \
		elif [ "$(UNAME_S)" == "Linux" ] && [ "$(UNAME_M)" == "x86_64" ];then \
			if [ -d "$(RAYLIB_FOLDER)" ]; then \
				echo "raylib already installed for your lib"; \
				exit 0; \
			else \
				echo "Downloading raylib 5.5 for Linux amd64..."; \
				mkdir $(RAYLIB_FOLDER); \
 				wget https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_linux_amd64.tar.gz; \
				tar -xzf raylib-5.5_linux_amd64.tar.gz -C $(RAYLIB_FOLDER) --strip-component 1; \
				rm raylib-5.5_linux_amd64.tar.gz; \
				echo "raylib downloaded and extracted"; \
			fi \
		else \
			echo "Error: This Makefile only supports Linux x86_64 (amd64) and MacOS"; \
			echo "Detected: $(UNAME_S) $(UNAME_M)"; \
			exit 1; \
		fi'

clean:
	rm -rf $(BUILD)
