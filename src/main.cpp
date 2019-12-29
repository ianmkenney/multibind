#include <iostream>
#include <string>

#include "IO.h"

double potential(double *energies)
{
	return -1;
}

int main(int argc, char const *argv[])
{	
	std::cout << "Beginning MC simulation" << std::endl;

	std::cout << "\tBuilding state defintion and connections" << std::endl;

	int Nstates  = count_states("../examples/input/4-state-diamond/states.csv");
	int Nconnect = count_connections("../examples/input/4-state-diamond/graph.csv");
	
	std::string names[Nstates];
	int connections[Nconnect * 2];
	double deltas[Nconnect * 2];
	double stdev[Nconnect * 2];
	
	collect_states("../examples/input/4-state-diamond/states.csv", names);
	std::cout << "\tFound " << Nstates << " states:" << std::endl;
	
	for (std::string sname : names)
	{
		std::cout << "\t\t" << sname << std::endl;
	}

	collect_connections("../examples/input/4-state-diamond/graph.csv", Nstates, names, connections, deltas, stdev);
	std::cout << "\tFound " << Nconnect << " connections:" << std::endl;

	for (int i = 0; i < Nconnect*2; ++i)
	{
		std::cout << "\t\t" << names[connections[i]] << " -- " << names[connections[i+1]] << std::endl;
		i++;
	}

	return 0;
}