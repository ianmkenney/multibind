#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

std::vector<int>* get_uv_neighbors(int, int, int, bool*, int*);
bool unvisited(bool*, int);
template <typename T> T get_pair_values(int, int, int*, T*, int, bool);
void dijkstra(int, int, double*, double*, int, int*, double*, double*);

void jacobian(double* jac, int* connections, double* stdevs, int Nconnections, int Nstates);
double* grad_log_liklihood(double* energies, double* stdevs);

double potential(double* deltas, double* stdevs);

#endif