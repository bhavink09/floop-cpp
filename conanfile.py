import os
from conan import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain, CMake, cmake_layout
from conan.tools.files import copy


class FloopCpp(ConanFile):
    name = "floop"
    version = "0.0.1"
    settings = "os", "compiler", "build_type", "arch"

    # Project configuration
    options = {"shared": [True, False], "fPIC": [True, False]}

    default_options = {"shared": False, "fPIC": True}
    exports_sources = "CMakeLists.txt", "src/*", "include/*"

    def package_info(self):
        self.cpp_info.libs = ["floop"]

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
        tc.variables["BUILD_EXAMPLES"] = "OFF"
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def configure(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")
        if self.options.shared:
            self.options.rm_safe("fPIC")
