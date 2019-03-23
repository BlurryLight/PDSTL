#project(test)
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/test")

set(PROJECT_TEST_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/test/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_TEST_BINARY_DIR})
set(CMAKE_CXX_STANDARD 11)
#set(TEST_LIST test.cpp alloc_test.cpp)

add_executable(test_alloc alloc_test.cpp)
add_test(test_alloc ${CMAKE_SOURCE_DIR}/build/test/test_alloc)
