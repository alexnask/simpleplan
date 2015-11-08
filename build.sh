#!/bin/sh

mkdir -p lib
mkdir -p obj

         #source/*.cpp
for f in source/*/*.cpp
do
    g++ -g -Wall -std=c++14 -Iinclude -c $f -o "${f%.*}.o"
    mv "${f%.*}.o" obj
done

ar rvs lib/libsimpleplan.a "obj/*.o"
