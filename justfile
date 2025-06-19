# Just configuration
set shell := ["bash", "-uc"]

# Default recipe - show available commands
default:
    @just --list


# Install dependencies for Debug build
conan-install-debug:
    conan install . --build=missing -s build_type=Debug

compile-commands: conan-install-debug
  cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON --preset conan-debug
  rm -rf compile_commands.json
  ln -s build/Debug/compile_commands.json .

# Install dependencies for Release build
conan-install:
    conan install . --build=missing -s build_type=Release

# Configure for Debug build
configure-debug: conan-install-debug
    cmake --preset conan-debug

# Configure for Release build
configure: conan-install
    cmake --preset conan-release

# Build the project (Release)
build: 
    cmake --build --preset conan-release

# Build Debug version
build-debug: 
    cmake --build --preset conan-debug

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

# Install Conan profile if it doesn't exist
setup-conan:
    conan profile detect --force

# Format code (if you have clang-format)
format:
    find src include tests -name "*.cpp" -o -name "*.hpp" -o -name "*.h" | xargs clang-format -i

create:
    conan create . --profile=default --build=missing -v
