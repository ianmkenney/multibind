#include "IO.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <math.h>
#include <map>

bool file_exists(char const* fname)
{
	std::ifstream infile(fname);
	if (infile)
	{
		infile.close();
		return true;
	}
	infile.close();
	return false;
}

/*
Read the contents of any csv that uses '#' as a comment character
and returns the number of entries.
*/
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

std::map<std::string, double>* get_concentrations(char const *fname)
{
	std::ifstream infile(fname);
	std::string line, entry;
	std::string lig;
	double concentration;
	std::map<std::string, double>* conc = new std::map<std::string, double>;

	while(getline(infile, line))
	{
		if (line[0] == '#' || line[0] == '\n') continue;
		std::stringstream ss(line);
		getline(ss, entry, ',');
		lig = entry;
		getline(ss, entry, ',');
		concentration = std::stod(entry);
		(*conc).insert(std::pair<std::string, double>(lig, concentration));
	}

	infile.close();
	return conc;
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
			int convert = std::stoi(entry);
			values[counter] = convert;
		}

		counter++;
	}
	infile.close();
	return values;
}

template double* graph_entries<double*, double>(char const*, int, int);
template int* graph_entries<int*, int>(char const*, int, int);

template <>
std::string* graph_entries<std::string*, std::string>(char const* fname, int column, int lines)
{
	std::ifstream infile(fname);
	std::string line, entry;

	std::string* values = new std::string[lines];

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

		std::string convert = entry;
		values[counter] = convert;

		counter++;
	}
	infile.close();
	return values;
}

int* collect_connections(char const* fname, int Nstates, std::string* names, int* Nconnect)
{
	*Nconnect = count_entries(fname);
	int* connections = new int[*Nconnect*2];

	std::string* s1 = graph_entries <std::string*, std::string> (fname, 0, *Nconnect);
	std::string* s2 = graph_entries <std::string*, std::string> (fname, 1, *Nconnect);

	for (int i = 0; i < *Nconnect; ++i)
	{
		connections[i*2] = get_index(names, s1[i], Nstates);
		connections[i*2 + 1] = get_index(names, s2[i], Nstates);
	}

	return connections;
}

double* collect_deltas(char const* fname, double pH, std::map<std::string, double> &concentrations)
{
	int Nconnect = count_entries(fname);
	double* delta_raw = graph_entries <double*, double> (fname, 2, Nconnect);
	std::string* ligand = graph_entries <std::string*, std::string> (fname, 4, Nconnect);
	double* standard = graph_entries <double*, double> (fname, 5, Nconnect);

	double* deltas = new double[Nconnect];
	std::string lig;

	for (int i = 0; i < Nconnect; ++i)
	{
		lig = ligand[i];
		if (lig == "H+") deltas[i] = log(10) * (pH - delta_raw[i]);
		else if (lig == "helm") deltas[i] = delta_raw[i];
		else deltas[i] = delta_raw[i] - log(concentrations[lig]/standard[i]);
	}
	return deltas;
}

double* collect_stdevs(char const* fname, double pH)
{
	int Nconnect = count_entries(fname);
	double* stdev_raw = graph_entries <double*, double> (fname, 3, Nconnect);

	for (int i = 0; i < Nconnect; ++i)
	{
		stdev_raw[i] = sqrt(stdev_raw[i]);
	}

	std::string* ligand = graph_entries <std::string*, std::string> (fname, 4, Nconnect);
	double* standard = graph_entries <double*, double> (fname, 5, Nconnect);

	double* stdev = new double[Nconnect];
	std::string lig;

	for (int i = 0; i < Nconnect; ++i)
	{
		lig = ligand[i];
		if (lig == "H+") stdev[i] = log(10) * stdev_raw[i];
		else if (lig == "helm") stdev[i] = stdev_raw[i];
		else stdev[i] = stdev_raw[i];
	}


	return stdev;
}

int get_index(std::string *names, std::string name, int Nstates)
{
	for (int i=0 ; i < Nstates; i++)
	{
		if (names[i] == name) return i;
	}
	return -1;
}