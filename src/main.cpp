#include <iostream>
#include <string>
#include <map>
#include <math.h>

#include "IO.h"
#include "graph.h"

int main(int argc, char const *argv[])
{	
	int Nstates, Nconnect;
	double pH = 8.0;

	std::cout << "Beginning MC simulation at pH = " << pH << std::endl;
	std::cout << "\tBuilding state defintion and connections" << std::endl;

	std::map<std::string, double>* concentrations;

	std::string concentraion_file = "../examples/input/4-state-diamond/concentrations.csv";

	if (file_exists(concentraion_file))
	{
		std::map<std::string, double>* concentrations = get_concentrations("../examples/input/4-state-diamond/concentrations.csv");
	}
	else
	{
		std::cout << "\tLoading default concentrations\n";
	}

	std::string* names  = collect_states("../examples/input/4-state-diamond/states.csv", &Nstates);
	int* connections = collect_connections("../examples/input/4-state-diamond/graph.csv", Nstates, names, &Nconnect);
	double* deltas = collect_deltas("../examples/input/4-state-diamond/graph.csv", pH, *concentrations);
	double* stdev = collect_stdevs("../examples/input/4-state-diamond/graph.csv", pH);
	


	std::cout << "\n\tFound " << Nstates << " states:" << std::endl;
	for (int i = 0; i < Nstates; ++i)
	{
		std::cout << "\t\t" << names[i] << std::endl;
	}
	

	std::cout << "\n\tFound " << Nconnect << " edges:" << std::endl;
	for (int i = 0; i < Nconnect; ++i)
	{
		std::cout << "\t\t" <<  names[connections[i*2]] << " <--> " << names[connections[i*2 + 1]];
		std::cout << "   --->   " << deltas[i] << " (" << pow(stdev[i],2) << ") kT\n";
	}

	double ddeltas[Nstates];
	double dvars[Nstates];

	int ref = 0;

	dijkstra(Nstates, Nconnect, ddeltas, dvars, ref, connections, deltas, stdev);

	std::cout << "\n\tPerforming Dijkstra shortest path algorithm relative to state " << ref << std::endl;
	std::cout << "\tMinimizing by variance" << std::endl;
	std::cout << "\t\tName" << "\tEnergy" << "\tVar\n";

	for (int i = 0; i < Nstates; ++i)
	{
		std::cout << "\t\t" << names[i] << "\t" << ddeltas[i] << "\t" << dvars[i] << std::endl;
	}

	double jac[Nstates * Nconnect];

	jacobian(jac, connections, stdev, Nconnect, Nstates);

	std::cout << std::endl;

	for (int k = 0; k < Nconnect; ++k)
	{
		for (int m = 0; m < Nstates; ++m)
		{
			std::cout << jac[m*Nconnect + k] << "   ";
		}
		std::cout << '\n';
	}

	return 0;
}