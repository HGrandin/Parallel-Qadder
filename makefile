CC=g++
CFLAGS=-I.
CXXFLAGS  = $(CFLAGS) -std=c++11

tests: kogge_stone.o propgen.o initial_propgen.o tests.o 
	$(CC) -o tests tests.o initial_propgen.o propgen.o kogge_stone.o $(CXXFLAGS)

.PHONY: clean

clean: 
	-rm *.o