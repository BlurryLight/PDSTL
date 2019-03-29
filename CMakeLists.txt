cmake_minimum_required(VERSION 2.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_STANDARD_REQUIRED 11)
project(PDSTL)
set(PROJECT_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

add_subdirectory(test)

enable_testing()

add_test(test_alloc ${CMAKE_SOURCE_DIR}/build/test/test_alloc)
add_test(test_smartptr ${CMAKE_SOURCE_DIR}/build/test/test_smartptr)
add_test(test_type_traits ${CMAKE_SOURCE_DIR}/build/test/test_type_traits)
add_test(test_vector ${CMAKE_SOURCE_DIR}/build/test/test_vector)
