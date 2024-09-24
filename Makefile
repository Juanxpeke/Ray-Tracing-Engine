# Set default target to build
.PHONY: all build run clean

all: build

# Commands for compiling
build-project:
	cmake -B Build

build-debug: build-project
	cmake --build Build --config Debug

build-release: build-project
	cmake --build Build --config Release

build: build-debug

# Commands for running Main.exe
run-debug:
	@echo "Running Build/Debug/Main.exe and outputting to Images/Main.ppm"
	@Build\Debug\Main.exe > Images/Main.ppm

run-release:
	@echo "Running Release/Debug/Main.exe and outputting to Images/Main.ppm"
	@Build\Release\Main.exe > Images/Main.ppm

run: run-debug

# Clean up build folder
clean:
	@rm -rf Build