cmake_minimum_required(VERSION 2.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_STANDARD_REQUIRED C11)
project(PDSTL)
set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin)
add_subdirectory(src)
add_subdirectory(test)

