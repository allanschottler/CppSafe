from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

class Conan(ConanFile):
    name = "sayf"
    version = "1.0"
    license = "MIT"
    author = "Allan Schöttler"
    #url = "https://github.com/allanschottler/GLPL"
    description = "Safe Cpp"
    topics = ("C++")
    
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"
    
    requires = [
        "boost/1.85.0",
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

    def package_info(self):
        self.cpp_info.libs = ["sayf"]
    
