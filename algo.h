#ifndef ALGO_H
#define ALGO_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <unordered_set>

#include "graph.h"
#include "simulator.h"

class algo
{

private:
    int minKey_index(std::vector<double> key, std::vector<bool> mstSet, int INT_MAX, int number_of_nodes);
    int find_in_subtree(std::vector<std::pair<double, std::unordered_set<int>>> sub_trees, int who);
    void DFSUtil(int v, std::vector<int> tree, std::vector<bool> visited, std::vector<int> &sol);

public:
    algo();
    ~algo();

    std::vector<int> primMST(graph G, std::vector<int> forced_nodes, double budget);
    std::vector<int> metric_k_center(graph G, int k);
    std::vector<int> find_TSP(int start, std::vector<int> tree);
};

#endif // ALGO_H