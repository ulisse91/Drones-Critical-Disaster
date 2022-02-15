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
    int n_depots;
    // double budget;
    // double prob_sigma_prime;
    long seed;

public:
    simulator_md(graph _G, std::vector<std::tuple<int, int, double>> drones, int _n_depots, long _seed);
    ~simulator_md();

    bool check_feasibility_multi_depot();
};

#endif // SIMULATOR_MULTI_DEPOT_H