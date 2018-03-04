#!/bin/bash

mkdir -p ./build
pushd ./build

CommonFlags="-Wall -Werror -Wno-write-strings -Wno-unused-variable -Wno-sign-compare -std=gnu++11 -fno-rtti -fno-exceptions"

# Build a 64-bit version
c++ $CommonFlags ../main.cpp -o main.x86_64 -l SDL2 -g

popd
