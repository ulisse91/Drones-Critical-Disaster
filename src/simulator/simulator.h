#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>
#include <vector>
#include <assert.h>
#include <utility>
#include <map>
#include <unordered_set>

#include "../core/graph.h"
#include "../core/algo.h"
#include "../core/print.h"
#include "utilities.h"
#include "prim_based.h"
#include "top_based.h"
#include "greedy.h"

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

    std::map<int, int> extract_sub_tree(std::map<int, int>, int start);

public:
    simulator(graph _G, int _n_drones, int _n_batteries, double _budget);
    ~simulator();

    int check_solution_feasible(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    double evaluate_solution(int which, std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    bool check_feasibility();

    // algorithms
    std::vector<std::vector<std::vector<std::pair<int, double>>>> prim_based_alg();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> top_based_alg();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> greedy_based_alg();
};

#endif // SIMULATOR_H