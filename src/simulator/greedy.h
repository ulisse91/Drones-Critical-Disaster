#ifndef GREEDY_H
#define GREEDY_H

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

class greedy
{

private:
    graph G;
    int n_drones;
    int n_batteries;
    double budget;

    int priority_max = 3,
        priority_med = 2,
        priority_min = 1;

    std::vector<int> greedy_find_path(std::unordered_set<int> graph_vertices, bool max);

public:
    greedy(graph _G, int _n_drones, int _n_batteries, double _budget);
    ~greedy();

    std::vector<std::vector<std::vector<std::pair<int, double>>>> greedy_algorithm(bool max);
};

#endif // GREEDY_H