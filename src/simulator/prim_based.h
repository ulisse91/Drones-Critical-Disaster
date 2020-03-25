#ifndef PRIM_BASED_H
#define PRIM_BASED_H

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

class primb
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

    void clean_sol(std::vector<std::vector<int>> &_temp);

public:
    primb(graph _G, int _n_drones, int _n_batteries, double _budget, std::map<int, int> _sigma_prime_probs, long _seed);
    ~primb();

    std::vector<std::vector<std::vector<std::pair<int, double>>>> prim_based_alg(bool sigma_prime);
};

#endif // PRIM_BASED_H