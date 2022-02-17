#ifndef SIMULATOR_MULTI_DEPOT_H
#define SIMULATOR_MULTI_DEPOT_H

#include <iostream>
#include <vector>
#include <assert.h>
#include <utility>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include <tuple>

#include "../core/graph.h"
#include "../core/algo.h"
#include "../core/print.h"
#include "utilities.h"

class simulator_md
{

private:
    graph G;
    std::vector<std::tuple<int, int, double>> drones;
    int n_batteries;

public:
    simulator_md(graph _G, std::vector<std::tuple<int, int, double>> drones);
    ~simulator_md();

    bool check_feasibility_multi_depot();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> meta_algorithm(int which_alg);
    std::vector<std::vector<int>> calculate_cycles_round(int which_alg, std::unordered_set<int> graph_vertices);
    std::vector<std::vector<int>> greedy_round(std::unordered_set<int> graph_vertices);
    int check_solution_feasible(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    double evaluate_solution(int which, std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    double objective_function_cycle(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    double objective_function_completion_time(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
};

#endif // SIMULATOR_MULTI_DEPOT_H