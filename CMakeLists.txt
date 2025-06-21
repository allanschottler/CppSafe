cmake_minimum_required(VERSION 3.16)
project(sayf CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++23")

find_package(Boost CONFIG REQUIRED)

include(CTest)
enable_testing()

include_directories(include)

# Add subdirectories
add_subdirectory(test)
