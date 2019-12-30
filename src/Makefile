all: IO.o graph.o
	g++ *.o main.cpp -o multibind-mc

IO.o:
	g++ -c IO.cpp 

graph.o:
	g++ -c graph.cpp

clean:
	rm *.o multibind-mc