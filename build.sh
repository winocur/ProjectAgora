#!/bin/bash
clear
mkdir -p bin
pushd bin
c++ -w ../src/sdl_nova.cpp -o nova -g `sdl2-config --cflags --libs`
popd
