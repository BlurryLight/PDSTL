#!/bin/bash

rm -rf build
export CXX=g++
export CC=gcc
mkdir build && cd build
cmake ..
make -j4
ctest
