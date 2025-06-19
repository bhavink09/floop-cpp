# Just configuration
set shell := ["bash", "-uc"]

# Default recipe - show available commands
default:
    @just --list

compile-commands:
  cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build

# Install Conan dependencies
install:
    conan install . --build=missing

# Install dependencies for Debug build
install-debug:
    conan install . --build=missing -s build_type=Debug

# Install dependencies for Release build
install-release:
    conan install . --build=missing -s build_type=Release

# Configure CMake project
configure: install
    cmake --preset conan-default

# Configure for Debug build
configure-debug: install-debug
    cmake --preset conan-debug

# Configure for Release build
configure-release: install-release
    cmake --preset conan-release

# Build the project (Release)
build: configure-release
    cmake --build --preset conan-release

# Build Debug version
build-debug: configure-debug
    cmake --build --preset conan-debug

# Build Release version
build-release: configure-release
    cmake --build --preset conan-release

# Clean build files
clean:
    rm -rf build/
    rm -rf CMakeUserPresets.json

# Run tests
test: build
    ctest --preset conan-release --verbose

# Run tests (Debug)
test-debug: build-debug
    ctest --preset conan-debug --verbose

# Run the main executable
run: build
    ./build/Release/MyProject

# Run the main executable (Debug)
run-debug: build-debug
    ./build/Debug/MyProject

# Full clean build and test cycle
all: clean build test

# Development cycle - quick build and test
dev: build-debug test-debug

# Install Conan profile if it doesn't exist
setup-conan:
    conan profile detect --force

# Show Conan info
info:
    conan info . --only=requires

# Create source and test directories if they don't exist
init:
    mkdir -p src include tests
    @echo "Created project directories: src/, include/, tests/"

# Format code (if you have clang-format)
format:
    find src include tests -name "*.cpp" -o -name "*.hpp" -o -name "*.h" | xargs clang-format -i

# Docker build (if you want containerized builds)
docker-build:
    docker run --rm -v $(pwd):/workspace -w /workspace conanio/gcc11 bash -c "just setup-conan && just all"
