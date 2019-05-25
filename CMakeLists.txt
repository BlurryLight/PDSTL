cmake_minimum_required(VERSION 2.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_STANDARD_REQUIRED 11)
project(PDSTL)
set(PROJECT_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
OPTION(USE_ALLOCATOR_MEMPOOL "Build the project with mempool version Allocator" OFF)

IF(USE_ALLOCATOR_MEMPOOL)
add_definitions("-DALLOCATOR_MEMPOOL")
endif(USE_ALLOCATOR_MEMPOOL)

add_subdirectory(test)

enable_testing()

add_test(test_alloc ${CMAKE_SOURCE_DIR}/build/test/test_alloc)
add_test(test_smartptr ${CMAKE_SOURCE_DIR}/build/test/test_smartptr)
add_test(test_type_traits ${CMAKE_SOURCE_DIR}/build/test/test_type_traits)
add_test(test_vector ${CMAKE_SOURCE_DIR}/build/test/test_vector)
add_test(test_list ${CMAKE_SOURCE_DIR}/build/test/test_list)
add_test(test_pair ${CMAKE_SOURCE_DIR}/build/test/test_pair)
