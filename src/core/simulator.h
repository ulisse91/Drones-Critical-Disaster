#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>
#include <vector>
#include <assert.h>
#include <utility>
#include <map>
#include <unordered_set>

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
    std::unordered_set<int> set_difference(std::unordered_set<int>, std::vector<int>);

public:
    simulator(graph _G, int _n_drones, int _n_batteries, double _budget);
    ~simulator();

    int check_solution_feasible(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    double evaluate_solution(int which, std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    void print_solution(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    void prim_based_alg();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> top_path_BB();
    double cost_cycle_OP(std::unordered_set<int> tsp_temp);
    double cost_budget_cycle(std::unordered_set<int> tsp_temp);
    double cost_budget_cycle(std::vector<int> _temp);
    std::vector<int> set_to_tsp(std::unordered_set<int> _temp);
    std::unordered_set<int> op_path_BB_insert_step(std::unordered_set<int> graph_vertices, std::unordered_set<int> sol_temp);
};

#endif // SIMULATOR_H