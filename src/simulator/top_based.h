#ifndef TOP_BASED_H
#define TOP_BASED_H

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

class topb
{

private:
    graph G;
    int n_drones;
    int n_batteries;
    double budget;
    std::map<int, int> sigma_prime_probs;
    long seed;

    int priority_max = 3,
        priority_med = 2,
        priority_min = 1;

public:
    topb(graph _G, int _n_drones, int _n_batteries, double _budget, std::map<int, int> _sigma_prime_probs, long _seed);
    ~topb();

    double cost_cycle_OP(std::unordered_set<int> tsp_temp);

    std::vector<std::vector<std::vector<std::pair<int, double>>>> top_path_BB(bool sigma_prime);
    std::unordered_set<int> op_path_BB_insert_step(std::unordered_set<int> graph_vertices, std::unordered_set<int> sol_temp);
};

#endif // TOP_BASED_H