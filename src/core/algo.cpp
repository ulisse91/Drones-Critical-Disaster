#include "algo.h"

algo::algo() {}
algo::~algo() {}

/////////////////////////////////////////////////////////////////////
//////////////////////// UTILITIES //////////////////////////////////
/////////////////////////////////////////////////////////////////////

bool compare(const std::pair<int, int> &a, const std::pair<int, int> &b)
{
    return a.second > b.second;
}

int algo::minKey_index(std::vector<double> key, std::vector<bool> mstSet, int INT_MAX, int number_of_nodes)
{
    double min = INT_MAX;
    int min_index = -1;
    for (int v = 0; v < number_of_nodes; v++)
    {
        if (mstSet[v] == false and key[v] <= min)
        {
            min = key[v];
            min_index = v;
        }
    }
    return min_index;
}

int algo::find_in_subtree(std::vector<std::pair<double, std::unordered_set<int>>> sub_trees, int who)
{
    for (size_t j = 0; j < sub_trees.size(); j++)
    {
        if (sub_trees[j].second.find(who) != sub_trees[j].second.end())
            return j;
    }
    return -1;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////// MAIN ////////////////////////////////////
/////////////////////////////////////////////////////////////////////

std::vector<int> algo::primMST(graph G, std::vector<int> forced_nodes, double budget)
{
    std::vector<int> parent(G.get_n_nodes());
    std::vector<double> key(G.get_n_nodes());
    std::vector<bool> mstSet(G.get_n_nodes());
    std::vector<std::pair<double, std::unordered_set<int>>> sub_trees;

    for (int i = 0; i < G.get_n_nodes(); i++)
    {
        key[i] = G.get_area_x() + G.get_area_y();
        mstSet[i] = false;
        parent[i] = -1;
    }

    std::vector<int> id_vertices = G.get_vertices();

    //////////////////////////////////////////////
    for (size_t i = 0; i < forced_nodes.size(); i++)
    {
        parent[forced_nodes[i]] = 0;
        key[forced_nodes[i]] = G.dist(id_vertices[0], id_vertices[forced_nodes[i]]);
        std::unordered_set<int> myset = {forced_nodes[i]};
        sub_trees.push_back(std::make_pair(budget - key[i], myset));
    }
    parent[0] = -1;
    mstSet[0] = forced_nodes.size() > 1 ? true : false;

    // for (size_t i = 0; i < sub_trees.size(); i++)
    // {
    //     std::unordered_set<int>:: iterator it;
    //     std::cout<<"Set contents are:\n";
    //     for(it=sub_trees[i].second.begin();it!=sub_trees[i].second.end();it++)
    //         std::cout<<*it<<" ";
    //     std::cout<<std::endl;
    // }

    //////////////////////////////////////////////

    for (size_t i = 0; i < G.get_n_nodes(); i++)
    {
        int u = minKey_index(key, mstSet, G.get_area_x() + G.get_area_y(), G.get_n_nodes());
        if (u == -1)
        {
            break;
        }

        mstSet[u] = true;

        for (int v = 1; v < key.size(); v++)
        {
            if (find(forced_nodes.begin(), forced_nodes.end(), v) != forced_nodes.end())
            {
                continue;
            }
            int index = find_in_subtree(sub_trees, u);

            // std::cout << " " << v << " " << index << " " << sub_trees[index].first << " " << mstSet[v] << " " << G.dist(id_vertices[u], id_vertices[v]) << " " << key[v] << std::endl;

            if (mstSet[v] == false and G.dist(id_vertices[u], id_vertices[v]) < key[v])
            {
                if (sub_trees[index].first - G.dist(id_vertices[u], id_vertices[v]) >= 0)
                {
                    if (parent[v] != -1)
                    {
                        int index_p = find_in_subtree(sub_trees, v);
                        sub_trees[index_p].first += G.dist(id_vertices[v], id_vertices[parent[v]]);
                        sub_trees[index_p].second.erase(v);
                    }
                    sub_trees[index].first -= G.dist(id_vertices[u], id_vertices[v]);
                    sub_trees[index].second.insert(v);
                    parent[v] = u, key[v] = G.dist(id_vertices[u], id_vertices[v]);
                }
            }
        }
    }

    // std::cout << std::endl << "print finale:\n";
    // for (size_t i = 1; i < parent.size(); i++)
    // {
    //     std::cout << i << "-" << parent[i] << " : " << G.dist(G.vertices[i], G.vertices[parent[i]]) << std::endl;
    // }

    return parent;
}

std::vector<int> algo::metric_k_center(graph G, int k)
{
    std::vector<int> sol;
    std::vector<int> temp_nodes = G.get_vertices();
    std::vector<int> id_vertices = G.get_vertices();

    /* initialize random seed: */
    srand(time(NULL));
    sol.push_back(id_vertices[rand() % (G.get_n_nodes() - 1) + 1]);
    k--;

    while (k > 0)
    {
        std::vector<int> temp(G.get_n_nodes());
        for (size_t i = 0; i < temp_nodes.size(); i++)
        {
            double min_dist = G.get_area_x() + G.get_area_y();
            for (size_t j = 0; j < sol.size(); j++)
            {
                min_dist = std::min(min_dist, G.dist(temp_nodes[i], id_vertices[sol[j]]));
            }
            temp[temp_nodes[i]] = min_dist;
        }

        int new_center = max_element(temp.begin(), temp.end()) - temp.begin();
        sol.push_back(new_center);
        temp_nodes.erase(temp_nodes.begin() + new_center);
        k--;
    }

    // for (size_t i = 0; i < sol.size(); i++)
    // {
    //     std::cout << sol[i] << " ";
    // }
    // std::cout << std::endl;

    return sol;
}

void algo::DFSUtil(int v, std::vector<int> tree, std::vector<bool> visited, std::vector<int> &sol)
{
    sol.push_back(v);
    visited[v] = true;

    for (size_t i = 0; i < tree.size(); i++)
    {
        if (tree[i] == v and !visited[i])
        {
            DFSUtil(i, tree, visited, sol);
        }
    }
}

std::vector<int> algo::find_TSP(int start, std::vector<int> tree)
{
    std::vector<int> sol;
    std::vector<bool> visited(tree.size(), false);

    DFSUtil(start, tree, visited, sol);

    // std::cout << std::endl;
    // for (size_t i = 0; i < sol.size(); i++)
    // {
    //     std::cout << sol[i] << " ";
    // }
    // std::cout << std::endl;

    return sol;
}