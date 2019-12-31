all: IO.o graph.o
	g++ *.o main.cpp -o multibind-mc

IO.o: IO.h graph.cpp
	g++ -c IO.cpp 

graph.o: graph.h graph.cpp
	g++ -c graph.cpp

clean:
	rm *.o multibind-mc