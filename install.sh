#!/bin/bash

if [ -d "build" ]; then
  echo "-- Delete build folder --"
  rm -R build
fi

echo "-- Create build folder --"
mkdir build && cd build

echo "-- Run CMake --"
cmake ..

echo "-- Run Make --"
make