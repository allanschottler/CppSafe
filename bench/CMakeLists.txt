add_executable (BenchIterator iterator.cpp)
target_include_directories (BenchIterator PRIVATE ${BOOST_INCLUDE_DIRS})
target_link_libraries (BenchIterator PRIVATE Boost::boost)
add_test(NAME Iterator COMMAND $<TARGET_FILE:BenchIterator>)

add_executable (BenchVector vector.cpp)
target_include_directories (BenchVector PRIVATE ${BOOST_INCLUDE_DIRS})
target_link_libraries (BenchVector PRIVATE Boost::boost)
add_test(NAME Vector COMMAND $<TARGET_FILE:BenchVector>)

add_executable (BenchAlgorithm algorithm.cpp)
target_include_directories (BenchAlgorithm PRIVATE ${BOOST_INCLUDE_DIRS})
target_link_libraries (BenchAlgorithm PRIVATE Boost::boost)
add_test(NAME Algorithm COMMAND $<TARGET_FILE:BenchAlgorithm>)

add_executable (BenchOptional optional.cpp)
target_include_directories (BenchOptional PRIVATE ${BOOST_INCLUDE_DIRS})
target_link_libraries (BenchOptional PRIVATE Boost::boost)
add_test(NAME Optional COMMAND $<TARGET_FILE:BenchOptional>)

add_executable (BenchNumeric numeric.cpp)
target_include_directories (BenchNumeric PRIVATE ${BOOST_INCLUDE_DIRS})
target_link_libraries (BenchNumeric PRIVATE Boost::boost)
add_test(NAME Numeric COMMAND $<TARGET_FILE:BenchNumeric>)