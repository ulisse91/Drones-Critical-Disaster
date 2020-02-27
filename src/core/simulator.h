#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>
#include <vector>
#include <assert.h>
#include <utility>
#include <map>

#include "graph.h"
#include "algo.h"

class simulator
{

private:
    graph G;
    int n_drones;
    int n_batteries;
    double budget;

    int priority_max = 3,
        priority_med = 2,
        priority_min = 1;

    double objective_function_cycle(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    double objective_function_weighted_latency(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    std::vector<int> op_path_BB_insert_step(std::vector<int> graph_vertices, int i, std::vector<int> sol_temp);
    
public:
    simulator(graph, int, int, int);
    ~simulator();

    int check_solution_feasible(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    double evaluate_solution(int which, std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    void print_solution(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    void prim_based_alg();
    std::vector<int> top_path_BB();
};

#endif // SIMULATOR_H