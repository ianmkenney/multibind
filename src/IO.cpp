#include "IO.h"
#include <fstream>
#include <sstream>

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

int count_connections(char const* fname)
{
	std::ifstream infile(fname);

	std::string line;
	int counter = 0;

	while(getline(infile, line))
	{
		if (line[0] == '#' || line[0] == '\n') continue;
		counter++;
	}

	infile.close();
	return counter;
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

void collect_connections(char const* fname, int Nstates, std::string* names, int* con, double* deltas, double* stdev)
{
	std::ifstream infile(fname);
	std::string line, entry;
	std::string s1, s2, ligand;
	int s1i, s2i;
	double value, variance, standard_state;

	int counter = 0;

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
		con[counter] = s1i;
		con[counter+1] = s2i;
		counter = counter+2;
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