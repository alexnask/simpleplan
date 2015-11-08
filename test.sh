#!/bin/sh

g++ -g -Wall -Iinclude -Llib -std=c++14 test/test.cpp -o test/test.exe -lsimpleplan
cd test
./test.exe
cd ..
rm test/test.exe
