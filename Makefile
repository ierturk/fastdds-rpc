# Variables
CONAN_PROFILE_DEBUG_X86 = x86_Debug
CONAN_PROFILE_RELEASE_X86 = x86_Release
CONAN_PROFILE_DEBUG_X64 = x64_Debug
CONAN_PROFILE_RELEASE_X64 = x64_Release
BUILD_DIR_32 = build

# Default target
all: install_x86 install_x64

# Conan install for 32-bit
install_x86:
	@echo "Running Conan install for x86..."
	@conan install . --profile:host=$(CONAN_PROFILE_RELEASE_X86) --build=missing
	@conan install . --profile:host=$(CONAN_PROFILE_DEBUG_X86) --build=newer

# Conan install for 64-bit
install_x64:
	@echo "Running Conan install for x86..."
	@conan install . --profile:host=$(CONAN_PROFILE_RELEASE_X64) --build=missing
	@conan install . --profile:host=$(CONAN_PROFILE_DEBUG_X64) --build=newer

# Clean build directories
clean:
	@echo "Cleaning build directories..."
	@if [ -d $(BUILD_DIR) ]; then rm -rf $(BUILD_DIR); fi
