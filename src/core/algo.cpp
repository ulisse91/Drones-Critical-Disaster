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

int algo::minKey_index(std::map<int, double> key, std::map<int, bool> mstSet, int INT_MAX, int number_of_nodes)
{
    double min = INT_MAX;
    int min_index = -1;
    for (auto const &pair : key)
    {
        if (mstSet[pair.first] == false and key[pair.first] <= min)
        {
            min = key[pair.first];
            min_index = pair.first;
        }
    }
    return min_index;
}

int algo::find_in_subtree(std::map<int, std::pair<double, std::unordered_set<int>>> sub_trees, int who)
{
    for (auto const &pair : sub_trees)
    {
        if (sub_trees[pair.first].second.find(who) != sub_trees[pair.first].second.end())
            return pair.first;
    }
    return -1;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////// MAIN ////////////////////////////////////
/////////////////////////////////////////////////////////////////////

std::map<int, int> algo::primMST(graph G, std::vector<int> forced_nodes, double budget)
{
    std::map<int, int> parent;
    std::map<int, double> key;
    std::map<int, bool> mstSet;
    std::map<int, std::pair<double, std::unordered_set<int>>> sub_trees;
    std::vector<int> id_vertices = G.get_vertices();

    for (int i = 0; i < id_vertices.size(); i++)
    {
        key[id_vertices[i]] = G.get_area_x() + G.get_area_y();
        mstSet[id_vertices[i]] = false;
        parent[id_vertices[i]] = -1;
    }

    //////////////////////////////////////////////
    for (size_t i = 0; i < forced_nodes.size(); i++)
    {
        parent[forced_nodes[i]] = 0;
        key[forced_nodes[i]] = G.dist(0, forced_nodes[i]);
        std::unordered_set<int> myset = {forced_nodes[i]};
        sub_trees[forced_nodes[i]] = std::make_pair(budget - key[forced_nodes[i]], myset);
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

    ////////////////////////////////////////////

    for (size_t i = 0; i < G.get_n_nodes(); i++)
    {
        int u = minKey_index(key, mstSet, G.get_area_x() + G.get_area_y(), G.get_n_nodes());
        if (u == -1)
        {
            break;
        }

        mstSet[u] = true;

        for (auto const &pair : key)
        {
            if (pair.first == u or find(forced_nodes.begin(), forced_nodes.end(), pair.first) != forced_nodes.end())
            {
                continue;
            }
            int index = find_in_subtree(sub_trees, u);

            // std::cerr << " " << u << " " << std::flush
            //           << pair.first << " " << std::flush
            //           << index << " " << std::flush
            //           << sub_trees[index].first << " " << std::flush
            //           << mstSet[pair.first] << " " << std::flush
            //           << G.dist(u, pair.first) << " " << std::flush
            //           << key[pair.first] << std::endl
            //           << std::flush;

            if (mstSet[pair.first] == false and G.dist(u, pair.first) < key[pair.first])
            {

                if (parent[pair.first] != -1)
                {
                    int index_p = find_in_subtree(sub_trees, pair.first);
                    if (index == index_p)
                    {
                        if (sub_trees[index].first + G.dist(parent[pair.first], pair.first) - G.dist(u, pair.first) >= 0)
                        {
                            sub_trees[index_p].first += G.dist(pair.first, parent[pair.first]) - G.dist(u, pair.first);
                            parent[pair.first] = u, key[pair.first] = G.dist(u, pair.first);
                        }
                    }
                    else
                    {
                        if (sub_trees[index].first - G.dist(u, pair.first) >= 0)
                        {
                            sub_trees[index_p].first += G.dist(pair.first, parent[pair.first]);
                            sub_trees[index_p].second.erase(pair.first);
                            sub_trees[index].first -= G.dist(u, pair.first);
                            sub_trees[index].second.insert(pair.first);
                            parent[pair.first] = u, key[pair.first] = G.dist(u, pair.first);
                        }
                    }
                }
                else
                {
                    if (sub_trees[index].first - G.dist(u, pair.first) >= 0)
                    {
                        sub_trees[index].first -= G.dist(u, pair.first);
                        sub_trees[index].second.insert(pair.first);
                        parent[pair.first] = u, key[pair.first] = G.dist(u, pair.first);
                    }
                }

                // if (sub_trees[index].first - G.dist(u, pair.first) >= 0)
                // {
                //     if (parent[pair.first] != -1)
                //     {
                //         int index_p = find_in_subtree(sub_trees, pair.first);
                //         sub_trees[index_p].first += G.dist(pair.first, parent[pair.first]);
                //         sub_trees[index_p].second.erase(pair.first);
                //     }
                //     sub_trees[index].first -= G.dist(u, pair.first);
                //     sub_trees[index].second.insert(pair.first);
                //     parent[pair.first] = u, key[pair.first] = G.dist(u, pair.first);
                // }
            }
        }
    }

    // std::cout << std::endl
    //           << "print finale:\n";
    // for (auto const &pair : parent)
    // {
    //     std::cout << pair.first << ":" << pair.second << " ";
    // }
    // std::cout << std::endl;

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

void algo::DFSUtil(graph G, int v, std::map<int, int> tree, std::map<int, bool> visited, std::vector<int> &sol, double cost_cycle, double budget)
{
    // std::cerr << v << " : ";

    // for (size_t i = 0; i < sol.size(); i++)
    // {
    //     std::cerr << sol[i] << " ";
    // }
    // std::cerr << " | " << cost_cycle << std::endl;

    if (sol.size() > 0)
    {
        // std::cerr << " " << sol[sol.size() - 1] << " " << G.dist(sol[sol.size() - 1], v) << " " << G.dist(v, 0) << " " << budget << std::endl;

        if (cost_cycle + G.dist(sol[sol.size() - 1], v) + G.dist(v, 0) > budget)
            return;
        // std::cerr << " " << sol[sol.size() - 1] << " " << G.dist(sol[sol.size() - 1], v) << std::endl;
        cost_cycle += G.dist(sol[sol.size() - 1], v);
    }
    sol.push_back(v);
    visited[v] = true;
    for (auto const &pair : tree)
    {
        if (tree[pair.first] == v and not visited[pair.first])
        {
            DFSUtil(G, pair.first, tree, visited, sol, cost_cycle, budget);
        }
    }
}

std::vector<int> algo::find_TSP(graph G, double budget, int start, std::map<int, int> tree)
{
    std::vector<int> sol;
    std::map<int, bool> visited;
    for (auto const &pair : tree)
    {
        // std::cerr << pair.first << ":" << pair.second << " ";
        visited[pair.first] = false;
    }
    DFSUtil(G, start, tree, visited, sol, 0, budget);
    return sol;
}