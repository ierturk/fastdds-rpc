# Copyright (c) 2025, Ibrahim Erturk <me@erturk.me>, ErturkMe
# Licensed under the BSD 3-Clause License.
# See the LICENSE file in the project root for more information.

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.scm import Version
from conan.errors import ConanInvalidConfiguration

class FastDdsRpcQml(ConanFile):
    name = "fats-dds-rpc-qml"
    description = "Fast DDS RPC QML is a library that provides a QML interface for Fast DDS RPC, enabling easy integration of DDS-based communication in QML applications."
    version = "0.1.0"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False]
    }
    default_options = {
        "shared": False,
        "fPIC": True
    }

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def requirements(self):
        self.requires("fast-dds/2.14.3")
        self.requires("fast-cdr/2.1.0")

        if self.settings.arch != "x86":
            self.requires("qt/6.5.3")
            self.requires("boost/1.87.0")
            self.requires("gtest/1.16.0")

    def configure(self):
        if self.settings.arch != "x86":
            self.options["qt/*"].shared = True
            self.options["qt/*"].qtquickcontrols = True
            self.options["qt/*"].qtquickcontrols2 = True
            self.options["qt/*"].qtdeclarative = True
            self.options["qt/*"].qttools = True
            self.options["qt/*"].with_vulkan = False
            self.options["qt/*"].gui = True
            self.options["qt/*"].qtqmltools = True
            self.options["qt/*"].qtquicktools = True
            self.options["qt/*"].qtshadertools = True
            self.options["qt/*"].qttranslations = True
            self.options["qt/*"].qtsvg = True
            self.options["qt/*"].opengl = "desktop"
            self.options["qt/*"].qttest = True

            # Boost options
            self.options["boost/*"].shared = True
            self.options["boost/*"].header_only = False
            self.options["boost/*"].without_python = True
            self.options["boost/*"].without_graph = True
            self.options["boost/*"].without_test = False
            self.options["boost/*"].without_wave = True
            self.options["boost/*"].without_coroutine = False
            self.options["boost/*"].without_asio = False
            self.options["boost/*"].without_lockfree = False

    def layout(self):
        self.folders.build_folder_vars = ["settings.os", "settings.compiler", "settings.compiler.version", "settings.arch", "settings.build_type"]
        cmake_layout(self)

    def validate(self):
        # Validate compiler supports C++20
        if self.settings.compiler == "gcc" and Version(self.settings.compiler.version) < Version("10"):
            raise ConanInvalidConfiguration("GCC >= 10 is required for C++20 support")
        if self.settings.compiler == "clang" and Version(self.settings.compiler.version) < Version("10"):
            raise ConanInvalidConfiguration("Clang >= 10 is required for C++20 support")
        if self.settings.compiler == "msvc" and Version(self.settings.compiler.version) < Version("16"):
            raise ConanInvalidConfiguration("Visual Studio 2019 or higher is required for C++20 support")

    def generate(self):
        tc = CMakeToolchain(self)
        # tc.generator = "Visual Studio 17 2022"
        tc.generator = "Ninja"

        # Specify the output directories for Ninja
        tc.cache_variables["CMAKE_RUNTIME_OUTPUT_DIRECTORY"] = f"{self.build_folder}/bin"
        tc.cache_variables["CMAKE_LIBRARY_OUTPUT_DIRECTORY"] = f"{self.build_folder}/lib"
        tc.cache_variables["CMAKE_ARCHIVE_OUTPUT_DIRECTORY"] = f"{self.build_folder}/lib"

        # Add _WIN32_WINNT definition for Windows 10 or later
        if self.settings.os == "Windows":
            tc.preprocessor_definitions["_WIN32_WINNT"] = "0x0A00"

        tc.generate()

        # Generate CMake dependency files
        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        from conan.tools.cmake import CMake
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        from conan.tools.cmake import CMake
        cmake = CMake(self)
        cmake.install()

    def imports(self):
        # Copy shared libraries to the bin folder for Windows
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dll", dst="bin", src="lib")
        
        # For MacOS
        self.copy("*.dylib*", dst="bin", src="lib")
