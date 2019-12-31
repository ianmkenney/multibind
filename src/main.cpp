#include <iostream>
#include <string>

#include "IO.h"
#include "graph.h"

int main(int argc, char const *argv[])
{	
	std::cout << "Beginning MC simulation" << std::endl;
	std::cout << "\tBuilding state defintion and connections" << std::endl;

	int Nstates, Nconnect;
	
	std::string* names  = collect_states("../examples/input/4-state-diamond/states.csv", &Nstates);

	int* connections = collect_connections("../examples/input/4-state-diamond/graph.csv", &Nconnect);
	// double* deltas = collect_deltas("../examples/input/4-state-diamond/graph.csv")
	// double* stdev = collect_stdevs("../examples/input/4-state-diamond/graph.csv")
	
	
	

	std::cout << "\tFound " << Nstates << " states:" << std::endl;
	for (int i = 0; i < Nstates; ++i)
	{
		std::cout << "\t\t" << names[i] << std::endl;
	}
	

	std::cout << "\tFound " << Nconnect << " edges:" << std::endl;
	for (int i = 0; i < Nconnect; ++i)
	{
		std::cout << "\t\t" <<  connections[i*2] << " <--> " << connections[i*2 + 1] << std::endl;
	}
	return 0;
}