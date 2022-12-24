#! /bin/bash
g++ -c main.cpp match_server/*.cpp
g++ *.o -o main -lthrift
./main
