#ifndef ALGO_H
#define ALGO_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */

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
    vector<int> metric_k_center(graph G, int k);
};

#endif // ALGO_H