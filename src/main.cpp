#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "IO.h"
#include "graph.h"

int main(int argc, char const *argv[])
{	
	int Nstates, Nconnect;
	double pH = 7.0;

	std::cout << "Beginning MC simulation at pH = " << pH << std::endl;
	std::cout << "\tBuilding state defintion and connections" << std::endl;

	std::map<std::string, double>* concentrations;

	std::ifstream ifile("../examples/input/4-state-diamond/concentrations.csv");
	if (ifile)
	{
		std::map<std::string, double>* concentrations = get_concentrations("../examples/input/4-state-diamond/concentrations.csv");
	}
	else
	{
		std::cout << "\tLoading default concentrations\n";
	}
	ifile.close();

	std::string* names  = collect_states("../examples/input/4-state-diamond/states.csv", &Nstates);
	int* connections = collect_connections("../examples/input/4-state-diamond/graph.csv", &Nconnect);
	double* deltas = collect_deltas("../examples/input/4-state-diamond/graph.csv", pH, *concentrations);
	double* stdev = collect_stdevs("../examples/input/4-state-diamond/graph.csv", pH);
	


	std::cout << "\tFound " << Nstates << " states:" << std::endl;
	for (int i = 0; i < Nstates; ++i)
	{
		std::cout << "\t\t" << names[i] << std::endl;
	}
	

	std::cout << "\tFound " << Nconnect << " edges:" << std::endl;
	for (int i = 0; i < Nconnect; ++i)
	{
		std::cout << "\t\t" <<  connections[i*2] << " <--> " << connections[i*2 + 1];
		std::cout << "   --->   " << deltas[i] << " (" << stdev[i] << ") kT\n";
	}
	return 0;
}