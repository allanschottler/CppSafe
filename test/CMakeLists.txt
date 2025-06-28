add_executable (TestIterator iterator.cpp)
target_include_directories (TestIterator PRIVATE ${BOOST_INCLUDE_DIRS})
target_link_libraries (TestIterator PRIVATE Boost::boost)
add_test(NAME Iterator COMMAND $<TARGET_FILE:TestIterator>)

add_executable (TestVector vector.cpp)
target_include_directories (TestVector PRIVATE ${BOOST_INCLUDE_DIRS})
target_link_libraries (TestVector PRIVATE Boost::boost)
add_test(NAME Vector COMMAND $<TARGET_FILE:TestVector>)

add_executable (TestAlgorithm algorithm.cpp)
target_include_directories (TestAlgorithm PRIVATE ${BOOST_INCLUDE_DIRS})
target_link_libraries (TestAlgorithm PRIVATE Boost::boost)
add_test(NAME Algorithm COMMAND $<TARGET_FILE:TestAlgorithm>)

add_executable (TestOptional optional.cpp)
target_include_directories (TestOptional PRIVATE ${BOOST_INCLUDE_DIRS})
target_link_libraries (TestOptional PRIVATE Boost::boost)
add_test(NAME Optional COMMAND $<TARGET_FILE:TestOptional>)

add_executable (TestNumeric numeric.cpp)
target_include_directories (TestNumeric PRIVATE ${BOOST_INCLUDE_DIRS})
target_link_libraries (TestNumeric PRIVATE Boost::boost)
add_test(NAME Numeric COMMAND $<TARGET_FILE:TestNumeric>)