#ifndef IO_H
#define IO_H

#include <string>

int count_entries(char const*);

std::string* collect_states(char const*, int*);
int* collect_connections(char const*, int*);
double* collect_deltas(char const*);
double* collect_stdevs(char const*);
template <typename T, typename U> T graph_entries(char const*, int, int);

int get_index(std::string*, std::string name, int);

// template int* graph_entries<int*, int>(char const*, int, int);

// template std::string* graph_entries<std::string*, std::string>(char const*, int, int);


#endif