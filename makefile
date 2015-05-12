CC=g++
CFLAGS=-I.
CXXFLAGS  = $(CFLAGS) -std=c++11

tests: kogge_stone.o propgen.o half_adder.o tests.o 
	$(CC) -o tests tests.o half_adder.o propgen.o kogge_stone.o $(CXXFLAGS)

.PHONY: clean

clean: 
	-rm *.o