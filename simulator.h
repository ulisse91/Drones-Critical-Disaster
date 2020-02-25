#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>
#include <vector>
#include <assert.h>
#include <utility>

#include "graph.h"

class simulator
{

private:
    graph G;
    int n_drones;
    int n_batteries;
    int budget;

    double objective_function_cycle(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    double objective_function_weighted_latency(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);

public:
    simulator(graph, int, int, int);
    ~simulator();

    int check_solution_feasible(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    double evaluate_solution(int which, std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    void print_solution(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
};

#endif // SIMULATOR_H