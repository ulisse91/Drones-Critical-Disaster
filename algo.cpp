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

vector<int> algo::metric_k_center(graph G, int k)
{
    vector<int> sol;
    vector<pair<int, node>> temp_nodes;

    /* initialize random seed: */
    srand(time(NULL));
    sol.push_back(rand() % (G.n_nodes - 1) + 1);
    k--;

    for (size_t i = 0; i < G.n_nodes; i++)
    {
        temp_nodes.push_back(make_pair(i, G.vertices[i]));
    }

    while (k > 0)
    {
        vector<int> temp(G.n_nodes);
        for (size_t i = 0; i < temp_nodes.size(); i++)
        {
            double min_dist = G.area_x + G.area_y;
            for (size_t j = 0; j < sol.size(); j++)
            {
                min_dist = min(min_dist, G.dist(temp_nodes[i].second, G.vertices[sol[j]]));
            }
            temp[temp_nodes[i].first] = min_dist;
        }

        int new_center = max_element(temp.begin(), temp.end()) - temp.begin();
        sol.push_back(new_center);
        temp_nodes.erase(temp_nodes.begin() + new_center);
        k--;
    }

    // for (size_t i = 0; i < sol.size(); i++)
    // {
    //     cout << sol[i] << " ";
    // }
    // cout << endl;

    return sol;
}