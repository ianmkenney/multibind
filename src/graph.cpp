#include "graph.h"
#include <iostream>
#include <math.h>
#include <stdexcept>

std::vector<int>* get_uv_neighbors(int src, int Nconnect, int Nstates, bool* visited, int* connections)
{
	std::vector<int>* neighbors = new std::vector<int>;
	int s1, s2;

	for (int i = 0; i < Nconnect; ++i)
	{
		s1 = connections[2*i];
		s2 = connections[2*i + 1];

		if (s2 == src && !visited[s1]) neighbors->push_back(s1);
		if (s1 == src && !visited[s2]) neighbors->push_back(s2);
	}

	return neighbors;
}

/*
Determine if any states have not been visited
*/
bool unvisited(bool* visited, int Nstates)
{
	for (int i = 0; i < Nstates; ++i)
	{
		if (!visited[i]) return true;
	}
	return false;
}

template <typename T>
T get_pair_values(int i, int j, int* connections, T* values, int Nconnect, bool symmetric)
{
	for (int k = 0; k < Nconnect; ++k)
	{
		if ((connections[k*2] == i) && (connections[k*2+1] == j))
		{
			return values[k];
		}
		if ((connections[k*2+1] == i) && (connections[k*2] == j))
		{
			if (symmetric)
			{
				return values[k];
			}
			return -values[k];
		}
	}
	return -1;
}

void dijkstra(int Nstates, int Nconnect, double* ddelta, double* dvar, int src, int* connections, double* deltas, double* stdevs)
{
	std::vector<int>* neighbors;
	bool visited[Nstates];
	double energies[Nstates];
	double var[Nstates];
	double edge_var, edge_energy;
	int current = src;
	int neighbor_index;

	for (int i = 0; i < Nstates; ++i) visited[i] = false;
	for (int i = 0; i < Nstates; ++i) energies[i] = __INT_MAX__;
	for (int i = 0; i < Nstates; ++i) var[i] = __INT_MAX__;

	energies[current] = 0;
	var[current] = 0;

	while(unvisited(visited, Nstates))
	{
		neighbors = get_uv_neighbors(current, Nconnect, Nstates, visited, connections);
		int Nneighbors = neighbors->size();

		for (int i = 0; i < Nneighbors; ++i)
		{
			neighbor_index = (*neighbors)[i];
			edge_energy = get_pair_values(current, neighbor_index, connections, deltas, Nconnect, false);
			edge_var = pow(get_pair_values(current, neighbor_index, connections, stdevs, Nconnect, true), 2);
			if ((edge_var + var[current]) < var[neighbor_index])
			{
				var[neighbor_index] = edge_var + var[current];
				energies[neighbor_index] = edge_energy + energies[current];
			}
		}

		visited[current] = true;

		if (!unvisited(visited, Nstates)) break;

		int min_index = __INT_MAX__;
		double min_distance = __INT_MAX__;
		bool is_neighbor;

		for (int i = 0; i < Nstates; ++i)
		{
			if (!visited[i] && (var[i] < min_distance)) 
			{
				min_index = i;
				min_distance = var[i];
			}
		}

		if (min_index == __INT_MAX__) throw std::logic_error("Could not find an appropriate node");

		current = min_index;
	}
	for (int i = 0; i < Nstates; ++i)
	{
		ddelta[i] = energies[i];
		dvar[i] = var[i];
	}
}

void jacobian(double* jac, int* connections, double* stdevs, int Nconnections, int Nstates)
{
	int i, j;
	int djm, djk, dim, dik;
	int factor;

	for (int k = 0; k < Nconnections; ++k)
	{
		for (int m = 0; m < Nstates; ++m)
		{
			jac[m*Nconnections + k] = 0;
			for (int p = 0; p < Nconnections; ++p)
			{
				i = connections[p*2];
				j = connections[p*2 + 1];
				djm = j == m;
				djk = j == k;
				dim = i == m;
				dik = i == k;
				factor = djm*djk - dim*djk - djm*dik + dim*dik;
				jac[m*Nconnections + k] += -1/(stdevs[p] * stdevs[p]) * factor;
			}
		}
	}
}

void grad_log_liklihood(double* result, int* connections, double* energies, double* deltas, double* stdevs, int Nconnections)
{
	int i, j;
	double f, value;
	for (int m = 0; m < Nconnections; ++m)
	{
		i = connections[m*2];
		j = connections[m*2 + 1];
		value = - 1 / pow(stdevs[m],2) * (energies[j] - energies[i] - deltas[m]);
		result[i] = - value;
		result[j] = value;
	}
}

double* pseudo_inverse(double* matrix, int Nconnections, int Nstates)
{
	double* inverse = new double[Nconnections * Nstates];

	for (int i = 0; i < Nconnections*Nstates; ++i) inverse[i] = 0;

	return inverse;
}

double potential(double* deltas, double* stdevs)
{
	return -1;
}