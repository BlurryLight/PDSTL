#!/bin/bash

rm -rf build
export CXX=clang++
export CC=clang
mkdir build && cd build
cmake ..
make -j4
ctest
export CXX=g++
export CC=gcc
