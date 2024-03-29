#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

std::vector<int>* get_uv_neighbors(int, int, int, bool*, int*);
bool unvisited(bool*, int);
template <typename T> T get_pair_values(int, int, int*, T*, int, bool);
void dijkstra(int, int, double*, double*, int, int*, double*, double*);

void jacobian(double* jac, int* connections, double* stdevs, int Nconnections, int Nstates);
void grad_log_liklihood(double* result, int* connections, double* energies, double* deltas, double* stdevs, int Nconnections);

double* pseudo_inverse(double*, int, int);
double potential(double* deltas, double* stdevs);

#endif