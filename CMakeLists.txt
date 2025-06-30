cmake_minimum_required(VERSION 3.16)
project(CppSafe CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++23 -O3")

find_package(Boost CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)

include_directories(include)

add_subdirectory(benchmark)
add_subdirectory(test)
