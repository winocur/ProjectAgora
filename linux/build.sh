#!/bin/bash
mkdir -p bin
g++ -w ../src/sdl_crimson_spiral.cpp -o ../bin/crimson_spiral -g -lGL -lGLU `sdl2-config --cflags --libs`
echo Crimson Spiral Built!
