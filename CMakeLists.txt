cmake_minimum_required(VERSION 2.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_STANDARD_REQUIRED 11)
project(PDSTL)
set(PROJECT_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
add_subdirectory(src)
#add_subdirectory(test ${PROJECT_BINARY_DIR}/test/)
add_subdirectory(test)

enable_testing()

