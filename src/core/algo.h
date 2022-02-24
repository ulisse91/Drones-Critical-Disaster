#ifndef ALGO_H
#define ALGO_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <unordered_set>
#include <map>
#include <tuple>

#include "graph.h"
#include "print.h"

class algo
{

private:
    static int minKey_index(std::map<int, double> key, std::map<int, bool> mstSet, int max_int, int number_of_nodes);
    static int find_in_subtree(std::map<int, std::pair<double, std::unordered_set<int>>> sub_trees, int who);
    static void DFSUtil(graph G, int v, std::map<int, int> tree, std::map<int, bool> visited, std::vector<int> &sol, double &cost_cycle, double budget);
    static void DFSUtil_multi_depot(graph G, int v, int end, std::map<int, int> tree, std::map<int, bool> visited, std::vector<int> &sol, double &cost_cycle, double budget);

public:
    algo();
    ~algo();

    static std::map<int, int> primMST(graph G, std::vector<int> forced_nodes, double budget);
    static std::map<int, int> primMST_multi_depot(graph G, std::vector<int> forced_nodes, std::vector<std::tuple<int, int, double>> drones);
    static std::vector<int> metric_k_center(graph G, int k, long seed);
    static std::vector<int> find_TSP(graph G, double budget, int start, std::map<int, int> tree);
    static std::vector<int> find_TSP_multi_depot(graph G, double budget, int start, std::map<int, int> tree);
    static std::map<int, int> primMST_multi_depot_alternativo(graph G, int start, int budget, int number_of_depots);
};

#endif // ALGO_H