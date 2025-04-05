# Copyright (c) 2025, Ibrahim Erturk <me@erturk.me>, ErturkMe
# Licensed under the BSD 3-Clause License.
# See the LICENSE file in the project root for more information.

# Variables
BUILD_DIR = build
START_ENV_SCRIPT = start_env_and_vscode.bat

# Default target
all: install_x86 install_x64

# Conan install for 32-bit
install_x86:
	@echo "Running Conan install for x86..."
	@conan install . -s arch=x86 -s build_type=Release --build=missing
	@conan install . -s arch=x86 -s "&:build_type=Debug" -s build_type=Release

# Conan install for 64-bit
install_x64:
	@echo "Running Conan install for x64..."
	@conan install . -s arch=x86_64 -s build_type=Release --build=missing
	@conan install . -s arch=x86_64 -s "&:build_type=Debug" -s build_type=Release

# Start environment and open VS Code
start_env:
	@echo "Starting environment and opening VS Code for architecture: $(arch)..."
	@cmd.exe /c $(START_ENV_SCRIPT) $(arch)

# Clean build directories
clean:
	@echo "Cleaning build directories..."
	@if [ -d $(BUILD_DIR) ]; then rm -rf $(BUILD_DIR); fi
