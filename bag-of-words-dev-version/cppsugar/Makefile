CC = g++

all: demo
header: func.hpp util.hpp cppsugar
test: header test.cpp
	$(CC) test.cpp --std=c++11 -o test; \
	./test
demo: header demo.cpp 
	$(CC) demo.cpp --std=c++11 -o demo
