#ifndef ALGO_H
#define ALGO_H

#include <iostream>
#include <vector>

#include "graph.h"
#include "simulator.h"

using namespace std;

class algo
{

private:
    int minKey_index(vector<pair<int, double>> key, vector<bool> mstSet, int INT_MAX, int number_of_nodes);

public:
    algo();
    ~algo();

    vector<int> primMST(graph G);
};

#endif // ALGO_H