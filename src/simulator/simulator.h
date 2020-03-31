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

class simulator
{

private:
    graph G;
    int n_drones;
    int n_batteries;
    double budget;
    double prob_sigma_prime;
    long seed;

    int priority_max = 3,
        priority_med = 2,
        priority_min = 1;

    double objective_function_cycle(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    double objective_function_weighted_latency(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    double objective_function_completion_time(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);

    void update_sigma_prime();

public:
    std::map<int, int> sigma_prime_probs;

    simulator(graph _G, int _n_drones, int _n_batteries, double _budget, double prob_sigma_prime, long seed);
    ~simulator();

    int check_solution_feasible(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    double evaluate_solution(int which, std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    bool check_feasibility();

    // algorithms
    std::vector<std::vector<std::vector<std::pair<int, double>>>> prim_based_alg();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> top_plus_prim();

    std::vector<std::vector<std::vector<std::pair<int, double>>>> meta_algorithm(int which_alg);
    std::vector<int> calculate_cycles_round(int which_alg, std::unordered_set<int> graph_vertices);
    std::vector<int> check_cycle_sigma_prime_cycle(std::vector<int> cycle);

    std::vector<int> greedy_find_path(std::unordered_set<int> graph_vertices, bool max);
    double cost_cycle_OP(std::unordered_set<int> _temp);
    std::unordered_set<int> op_path_BB_insert_step(std::unordered_set<int> graph_vertices, std::unordered_set<int> sol_temp);
    std::vector<int> prim_based();

};

#endif // SIMULATOR_H