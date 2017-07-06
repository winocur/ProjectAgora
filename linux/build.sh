#!/bin/bash
mkdir -p ../bin
g++ -w ../src/sdl_crimson_spiral.cpp -o ../bin/crimson_spiral_linux -g -lalut -lopenal -lvorbisfile -lGL -lGLU \
       `sdl2-config --cflags --libs` -lSDL2_image -fno-exceptions
