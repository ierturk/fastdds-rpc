from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.scm import Version
from conan.errors import ConanInvalidConfiguration


class Conan2DdsTest(ConanFile):
    name = "conan2-dds-test"
    version = "1.0"

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

    def layout(self):
        cmake_layout(self)

        # Set architecture-specific build directories
        if self.settings.arch == "x86":
            self.folders.build = "build_proxy"
            self.folders.generators = "build_proxy"  # Place Conan-generated files in build_proxy
        elif self.settings.arch == "x86_64":
            self.folders.build = "build_invoker"
            self.folders.generators = "build_invoker"  # Place Conan-generated files in build_invoker

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
        tc.generator = "Visual Studio 17 2022"

        # Specify the output directories for Ninja
        # tc.cache_variables["CMAKE_RUNTIME_OUTPUT_DIRECTORY"] = f"{self.build_folder}/bin"
        # tc.cache_variables["CMAKE_LIBRARY_OUTPUT_DIRECTORY"] = f"{self.build_folder}/lib"
        # tc.cache_variables["CMAKE_ARCHIVE_OUTPUT_DIRECTORY"] = f"{self.build_folder}/lib"

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