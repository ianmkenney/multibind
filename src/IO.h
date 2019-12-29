#ifndef IO_H
#define IO_H

#include <string>

int count_states(char const*);
int count_connections(char const*);

void collect_states(char const*, std::string*);
void collect_connections(char const*, int, std::string*, int*, double*, double*);

int get_index(std::string*, std::string name, int);

#endif