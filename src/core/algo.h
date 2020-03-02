#ifndef ALGO_H
#define ALGO_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <unordered_set>
#include <map>

#include "graph.h"
#include "simulator.h"

class algo
{

private:
    static int minKey_index(std::map<int, double> key, std::map<int, bool> mstSet, int max_int, int number_of_nodes);
    static int find_in_subtree(std::map<int, std::pair<double, std::unordered_set<int>>> sub_trees, int who);
    static void DFSUtil(int v, std::map<int, int> tree, std::map<int, bool> visited, std::vector<int> &sol);

public:
    algo();
    ~algo();

    static std::map<int, int> primMST(graph G, std::vector<int> forced_nodes, double budget);
    static std::vector<int> metric_k_center(graph G, int k);
    static std::vector<int> find_TSP(int start, std::map<int, int> tree);
};

#endif // ALGO_H