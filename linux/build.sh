#!/bin/bash
clear
g++ -w ../src/sdl_crimson_spiral.cpp -o crimson_spiral -g -lGL -lGLU `sdl2-config --cflags --libs`
