#include "IO.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

int count_entries(char const *fname)
{
	std::ifstream infile(fname);
	std::string line;
	int entries = 0;
	while(getline(infile, line))
	{
		if (line[0] == '#' || line[0] == '\n') continue;
		entries++;
	}
	infile.close();
	return entries;
}

std::string* collect_states(char const *fname, int* Nstates)
{
	std::ifstream infile(fname);
	std::stringstream ss;
	std::string line;
	std::string entry;

	*Nstates = count_entries(fname);

	std::string* names = new std::string[*Nstates];


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
	return names;
}

template <typename T, typename U>
T graph_entries(char const* fname, int column, int lines)
{
	std::ifstream infile(fname);
	std::string line, entry;

	U* values = new U[lines];

	int counter = 0;

	while(getline(infile, line))
	{
		if (line[0] == '#' || line[0] == '\n') continue;
		std::stringstream ss(line);
		for (int i = 0; i < column; ++i)
		{
			getline(ss, entry, ',');
		}
		getline(ss, entry, ',');

		if (typeid(U) == typeid(double))
		{
			double convert = std::stod(entry);
			values[counter] = convert;
		}
		if (typeid(U) == typeid(int))
		{
			int convert = std::stod(entry);
			values[counter] = convert;
		}

		counter++;
	}
	infile.close();
	return values;
}

// void collect_connections(char const* fname, int* Nconnect)
// {
// 	std::ifstream infile(fname);
// 	std::string line, entry;
// 	std::string s1, s2, ligand;
// 	int s1i, s2i;
// 	double value, variance, standard_state;

// 	int counter = 0;

// 	while(getline(infile, line))
// 	{
// 		if (line[0] == '#' || line[0] == '\n') continue;
// 		std::stringstream ss;
// 		ss.str(line);
// 		getline(ss, entry, ',');
// 		s1 = entry;
// 		getline(ss, entry, ',');
// 		s2 = entry;
// 		getline(ss, entry, ',');
// 		value = std::stod(entry);
// 		getline(ss, entry, ',');
// 		variance = std::stod(entry);
// 		getline(ss, entry, ',');
// 		ligand = entry;
// 		getline(ss, entry, ',');
// 		standard_state = std::stod(entry);

// 		s1i = get_index(names, s1, Nstates);
// 		s2i = get_index(names, s2, Nstates);
// 		con[counter] = s1i;
// 		con[counter+1] = s2i;
// 		counter = counter+2;
// 	}

// 	infile.close();
// }

int* collect_connections(char const* fname,int* Nconnect)
{
	*Nconnect = count_entries(fname);
	int* connections = new int[*Nconnect*2];

	int* s1 = graph_entries <int*, int> ("../examples/input/4-state-diamond/graph.csv", 0, *Nconnect);
	int* s2 = graph_entries <int*, int> ("../examples/input/4-state-diamond/graph.csv", 1, *Nconnect);

	for (int i = 0; i < *Nconnect; ++i)
	{
		connections[i*2] = s1[i];
		connections[i*2 + 1] = s2[i];
	}

	return connections;
}

int get_index(std::string *names, std::string name, int Nstates)
{
	for (int i=0 ; i < Nstates; i++)
	{
		if (names[i] == name) return i;
	}
	return -1;
}

template double* graph_entries<double*, double>(char const*, int, int);
template int* graph_entries<int*, int>(char const*, int, int);