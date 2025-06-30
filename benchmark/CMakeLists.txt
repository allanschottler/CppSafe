add_executable (BenchOptional optional.cpp)
target_link_libraries (BenchOptional PRIVATE benchmark::benchmark)