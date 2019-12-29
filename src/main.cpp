#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

double potential(double *energies)
{
	return -1;
}

int count_states(char const *fname)
{
	std::ifstream infile(fname);
	std::string line;
	int states = 0;
	while(getline(infile, line))
	{
		if (line[0] == '#' || line[0] == '\n') continue;
		states++;
	}
	infile.close();
	return states;
}

void collect_states(char const *fname, std::string *names)
{
	std::ifstream infile(fname);
	std::stringstream ss;
	std::string line;
	std::string entry;

	int counter = 0;
	while(getline(infile, line))
	{
		if (line[0] == '#' || line[0] == '\n') continue;
		ss.str(line);
		getline(ss, entry, ',');
		names[counter] = entry;
		counter++;
	}
	infile.close();
}

int get_index(std::string *names, std::string name, int Nstates)
{
	for (int i=0 ; i < Nstates; i++)
	{
		if (names[i] == name) return i;
	}
	return -1;
}

void parameterize(char const *fname, std::string *names, int Nstates, int *connections, double *deltas, double *stdev)
{
	std::ifstream infile(fname);
	std::string line, entry;
	std::string s1, s2, ligand;
	int s1i, s2i;
	double value, variance, standard_state;

	while(getline(infile, line))
	{
		if (line[0] == '#' || line[0] == '\n') continue;
		std::stringstream ss;
		ss.str(line);
		getline(ss, entry, ',');
		s1 = entry;
		getline(ss, entry, ',');
		s2 = entry;
		getline(ss, entry, ',');
		value = std::stod(entry);
		getline(ss, entry, ',');
		variance = std::stod(entry);
		getline(ss, entry, ',');
		ligand = entry;
		getline(ss, entry, ',');
		standard_state = std::stod(entry);

		s1i = get_index(names, s1, Nstates);
		s2i = get_index(names, s2, Nstates);
		connections[s1i * Nstates + s2i ] = 1;
		connections[s2i * Nstates + s1i ] = 1;
	}
	infile.close();
}

int main(int argc, char const *argv[])
{	
	std::cout << "Beginning MC simulation" << std::endl;

	std::cout << "\tBuilding state defintion and connections" << std::endl;
	int Nstates = count_states("../examples/input/4-state-diamond/states.csv");
	std::string names[Nstates];
	collect_states("../examples/input/4-state-diamond/states.csv", names);
	std::cout << "\tFound " << Nstates << " states:" << std::endl;
	for (std::string sname : names)
	{
		std::cout << "\t\t" << sname << std::endl;
	}

	int connections[Nstates*Nstates];
	double deltas[Nstates*Nstates];
	double stdev[Nstates*Nstates];

	for (int i = 0; i < Nstates; ++i)
	{
		for (int j = 0; j < Nstates; ++j)
		{
			connections[i * Nstates + j] = 0;
			deltas[i * Nstates + j] = 0;
			stdev[i * Nstates + j] = 0;
		}
	}

	parameterize("../examples/input/4-state-diamond/graph.csv", names, Nstates, connections, deltas, stdev);

	for (int i = 0; i < Nstates; ++i)
	{
		for (int j = 0; j < Nstates; ++j)
		{
			std::cout << connections[j + i * Nstates] << " ";
		}
		std::cout << "\n";
	}

	return 0;
}