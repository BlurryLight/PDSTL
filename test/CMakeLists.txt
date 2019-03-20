INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/test")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/test)
set(CMAKE_CXX_STANDARD C11)
set(TEST_LIST test.cpp alloc_test.cpp)
add_executable(main ${TEST_LIST})

enable_testing()
add_test(main ${EXECUTABLE_OUTPUT_PATH}/test/main)
