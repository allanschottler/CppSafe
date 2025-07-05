from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

class Conan(ConanFile):
    name = "safepp"
    version = "1.0"
    license = "MIT"
    author = "Allan Schöttler"
    url = "https://github.com/allanschottler/CppSafe"
    description = "Safe Cpp"
    topics = ("C++")
    
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"
    exports_sources = [
        "CMakeLists.txt",
        "include/*",
        "benchmark/*",
        "test/*",
    ]
    
    requires = [
        "boost/1.85.0",
        "benchmark/1.9.1",
    ]
    
    def configure(self):
        pass

    def layout(self):
        # Use the default CMake layout
        cmake_layout(self)
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["safepp"]
    
