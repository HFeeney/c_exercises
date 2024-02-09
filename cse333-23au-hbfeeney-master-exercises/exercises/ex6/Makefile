all: ex6

ex6: ex6.o Vector.o
	g++ -Wall -g -std=c++17 -o ex6 ex6.o Vector.o

ex6.o: ex6.cc
	g++ -Wall -g -std=c++17 -c ex6.cc

Vector.o: Vector.cc
	g++ -Wall -g -std=c++17 -c Vector.cc

clean:
	rm ex6 *.o *~
