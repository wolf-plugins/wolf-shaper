CC=g++
binaries=Main.o TestGraph.o TestStack.o Graph.o

all: Graph.o tests

Graph.o: ../src/Graph.cpp
	$(CC) -c ../src/Graph.cpp -I../ -o Graph.o
	
tests: $(binaries)
	$(CC) -o tests $(binaries) $(INC) -lboost_unit_test_framework

clean:
	rm -f $(binaries) tests
