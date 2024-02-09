FLAGS = -Wall -std=c++17

ex7: ex7.o Vector.o
	g++ $(FLAGS) -o $@ $^

ex7.o: ex7.cc Vector.h
	g++ $(FLAGS) -c $<

Vector.o: Vector.cc Vector.h
	g++ $(FLAGS) -c $<

clean:
	rm ex7 *.o *~