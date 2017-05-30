#!/bin/bash
clear
mkdir -p bin
pushd bin
c++ -w ../src/sdl_crimson_spiral.cpp -o crimson_spiral -g `sdl2-config --cflags --libs`
popd
