#!/bin/bash
g++-6 -c caltree.cpp
g++-6 -Wall -w -o2 -o regression regression.cpp caltree.o
