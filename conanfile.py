from conan import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain, cmake_layout


class FloopCpp(ConanFile):
    name = "floop"
    version = "0.0.1"
    settings = "os", "compiler", "build_type", "arch"

    # Project configuration
    options = {"shared": [True, False], "fPIC": [True, False]}

    default_options = {"shared": False, "fPIC": True}

    def requirements(self):
        # Boost for boost::asio
        self.requires("boost/1.84.0")
        # spdlog for logging
        self.requires("spdlog/1.12.0")

    def build_requirements(self):
        # Google Test for unit testing
        self.test_requires("gtest/1.14.0")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        # Generate CMake dependencies
        deps = CMakeDeps(self)
        deps.generate()

        # Generate CMake toolchain
        tc = CMakeToolchain(self)
        tc.generator = "Ninja"
        tc.generate()

    def configure(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")
        if self.options.shared:
            self.options.rm_safe("fPIC")
