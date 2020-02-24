#include "algo.h"

algo::algo() {}
algo::~algo() {}

bool compare(const pair<int, int> &a, const pair<int, int> &b)
{
    return a.second > b.second;
}

int algo::minKey_index(vector<pair<int, double>> key, vector<bool> mstSet, int INT_MAX, int number_of_nodes)
{
    double min = INT_MAX;
    int min_index;
    for (int v = 0; v < number_of_nodes; v++)
    {
        if (mstSet[v] == false and key[v].second < min)
        {
            min = key[v].second;
            min_index = v;
        }
    }
    return min_index;
}

vector<int> algo::primMST(graph G)
{
    vector<int> parent(G.n_nodes);
    vector<pair<int, double>> key(G.n_nodes);
    vector<bool> mstSet(G.n_nodes);

    for (int i = 1; i < G.n_nodes; i++)
    {
        key[i] = make_pair(i, G.area_x + G.area_y);
        mstSet[i] = false;
        parent[i] = -1;
    }

    key[0] = make_pair(0, 0);
    parent[0] = -1;

    for (size_t i = 0; i < G.n_nodes; i++)
    {
        int u = minKey_index(key, mstSet, G.area_x + G.area_y, G.n_nodes);
        mstSet[u] = true;

        for (int v = 0; v < key.size(); v++)
        {
            if (mstSet[key[v].first] == false and G.dist(G.vertices[u], G.vertices[key[v].first]) < key[v].second)
            {
                parent[key[v].first] = u, key[v].second = G.dist(G.vertices[u], G.vertices[key[v].first]);
            }
        }
    }

    return parent;
}