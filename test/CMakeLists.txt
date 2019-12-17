project(test)
cmake_minimum_required(VERSION 2.8)
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/test")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src")

set(PROJECT_TEST_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/test/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_TEST_BINARY_DIR})
set(CMAKE_CXX_STANDARD 11)
#set(TEST_LIST test.cpp alloc_test.cpp)

add_executable(test_alloc alloc_test.cpp)
add_executable(test_smartptr smartptr_test.cpp)
add_executable(test_type_traits type_traits_test.cpp)
add_executable(test_iterator_traits iterator_test.cpp)
add_executable(test_vector vector_test.cpp)
add_executable(test_list list_test.cpp)
add_executable(test_pair pair_test.cpp)
add_executable(test_avl_tree avl_tree_test.cpp)
add_executable(test_map map_test.cpp)



