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
    // int n_batteries;
    int number_of_depots;

    std::vector<std::vector<int>> greedy_round(std::unordered_set<int> graph_vertices);

public:
    simulator_md(graph _G, std::vector<std::tuple<int, int, double>> drones, int _n_depots);
    ~simulator_md();

    bool check_feasibility_multi_depot();
    int check_solution_feasible(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    double evaluate_solution(int which, std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    double objective_function_cycle(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    double objective_function_completion_time(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    std::vector<std::vector<int>> prim_based(std::unordered_set<int> graph_vertices);
    std::vector<std::vector<std::vector<std::pair<int, double>>>> greedy_out_loop();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> kim_out_loop();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> kim_alternativo();
    std::vector<int> prim_based_alternativo(std::unordered_set<int> graph_vertices,int start, double budget, int current_drone);
};

#endif // SIMULATOR_MULTI_DEPOT_H