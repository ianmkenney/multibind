#ifndef IO_H
#define IO_H

#include <string>
#include <map>

bool file_exists(std::string);

int count_entries(std::string);
std::map<std::string, double>* get_concentrations(std::string fname);

std::string* collect_states(std::string, int*);
int* collect_connections(std::string, int, std::string*, int*);
double* collect_deltas(std::string, double, std::map<std::string, double>&);
double* collect_stdevs(std::string, double);
template <typename T, typename U> T graph_entries(std::string, int, int);

int get_index(std::string*, std::string name, int);

#endif