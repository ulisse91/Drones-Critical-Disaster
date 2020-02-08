#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>
#include <vector>
#include <assert.h>
#include <utility>

#include "graph.h"

using namespace std;

class simulator
{

private:
    graph G;
    int n_drones;
    int n_batteries;
    int budget;

public:
    simulator(graph, int, int, int);
    ~simulator();

    bool check_solution_feasible(vector<vector<vector<pair<int, double>>>> sol);
    double evaluate_solution(vector<vector<vector<pair<int, double>>>> sol);
    void print_solution(vector<vector<vector<pair<int, double>>>> sol);
};

#endif // SIMULATOR_H