# Variables
BUILD_DIR = build

# Default target
all: install_x86 install_x64

# Conan install for 32-bit
install_x86:
	@echo "Running Conan install for x86..."
	@conan install . -s arch=x86 -s build_type=Release
	@conan install . -s arch=x86 -s "&:build_type=Debug" -s build_type=Release

# Conan install for 64-bit
install_x64:
	@echo "Running Conan install for x64..."
	@conan install . -s arch=x86_64 -s build_type=Release
	@conan install . -s arch=x86_64 -s "&:build_type=Debug" -s build_type=Release

# Clean build directories
clean:
	@echo "Cleaning build directories..."
	@if [ -d $(BUILD_DIR) ]; then rm -rf $(BUILD_DIR); fi
