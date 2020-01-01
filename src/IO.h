#ifndef IO_H
#define IO_H

#include <string>
#include <map>

bool file_exists(char const*);

int count_entries(char const*);
std::map<std::string, double>* get_concentrations(char const *fname);

std::string* collect_states(char const*, int*);
int* collect_connections(char const*, int, std::string*, int*);
double* collect_deltas(char const*, double, std::map<std::string, double>&);
double* collect_stdevs(char const*, double);
template <typename T, typename U> T graph_entries(char const*, int, int);

int get_index(std::string*, std::string name, int);

#endif