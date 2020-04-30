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

    double _max = budget*2;

    for (auto const &i : id_vertices)
    {
        key[i] = _max;
        mstSet[i] = false;
        parent[i] = -1;
    }

    for (auto const &i : forced_nodes)
    {
        parent[i] = 0;
        key[i] = G.distw(0, i);
        std::unordered_set<int> myset = {i};
        sub_trees[i] = std::make_pair(budget - key[i], myset);
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

    for (int j = 0; j < G.get_n_nodes(); j++)
    {
        int u = minKey_index(key, mstSet, _max, G.get_n_nodes());
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
            //           << G.distw(u, pair.first) << " " << std::flush
            //           << key[pair.first] << std::endl
            //           << std::flush;

            if (mstSet[pair.first] == false and G.distw(u, pair.first) < key[pair.first])
            {

                if (parent[pair.first] != -1)
                {
                    int index_p = find_in_subtree(sub_trees, pair.first);
                    if (index == index_p)
                    {
                        if (sub_trees[index].first + G.distw(parent[pair.first], pair.first) - G.distw(u, pair.first) >= 0)
                        {
                            sub_trees[index_p].first += G.distw(pair.first, parent[pair.first]) - G.distw(u, pair.first);
                            parent[pair.first] = u, key[pair.first] = G.distw(u, pair.first);
                        }
                    }
                    else
                    {
                        if (sub_trees[index].first - G.distw(u, pair.first) >= 0)
                        {
                            sub_trees[index_p].first += G.distw(pair.first, parent[pair.first]);
                            sub_trees[index_p].second.erase(pair.first);
                            sub_trees[index].first -= G.distw(u, pair.first);
                            sub_trees[index].second.insert(pair.first);
                            parent[pair.first] = u, key[pair.first] = G.distw(u, pair.first);
                        }
                    }
                }
                else
                {
                    if (sub_trees[index].first - G.distw(u, pair.first) >= 0)
                    {
                        sub_trees[index].first -= G.distw(u, pair.first);
                        sub_trees[index].second.insert(pair.first);
                        parent[pair.first] = u, key[pair.first] = G.distw(u, pair.first);
                    }
                }
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

std::vector<int> algo::metric_k_center(graph G, int k, long seed)
{
    if (k == 0)
        return {};
    if (k == 1 or G.get_n_nodes() == 1)
        return {0};

    std::vector<int> sol;
    std::unordered_set<int> graph_vertices = G.get_vertices_set();
    graph_vertices.erase(graph_vertices.find(0));

    std::uniform_int_distribution<int> unif(1, G.get_n_nodes());
    std::mt19937 re(seed);

    // int _n = rand() % (G.get_n_nodes() - 1) + 1,
    int _n = unif(re), counter = 0;
    for (auto const &i : graph_vertices)
    {
        if (_n == counter)
        {
            sol.push_back(i);
            graph_vertices.erase(graph_vertices.find(i));
            break;
        }
        counter++;
    }

    while (sol.size() < (unsigned)k and graph_vertices.size() > 0)
    {
        std::map<int, double> temp;
        for (auto const &i : graph_vertices)
        {
            double min_dist = G.get_n_nodes() * (G.get_area_x() + G.get_area_y());
            for (size_t j = 0; j < sol.size(); j++)
            {
                if (i != sol[j])
                    min_dist = std::min(min_dist, G.distw(i, sol[j]));
            }
            temp[i] = min_dist;
        }
        double max_dist = -1;
        int new_center = -1;
        for (auto const &i : temp)
        {
            if (i.second > max_dist)
            {
                max_dist = i.second;
                new_center = i.first;
            }
        }
        if (new_center == -1)
            break;
        sol.push_back(new_center);
        graph_vertices.erase(graph_vertices.find(new_center));
    }
    return sol;
}

void algo::DFSUtil(graph G, int v, std::map<int, int> tree, std::map<int, bool> visited, std::vector<int> &sol, double &cost_cycle, double budget)
{
    if (sol.size() > 0)
    {
        //std::cerr << " " << sol[sol.size() - 1] << " " << G.distw(sol[sol.size() - 1], v) << " " << G.distw(v, 0) << " " << budget << std::endl;

        if (cost_cycle + G.distw(sol[sol.size() - 1], v) + G.distw(v, 0) > budget)
            return;
        cost_cycle += G.distw(sol[sol.size() - 1], v);

        //std::cerr << " " << cost_cycle << std::endl;
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
    // std::cout << "tree: "; print::print_map_int_int(tree);

    std::vector<int> sol;
    std::map<int, bool> visited;
    for (auto const &pair : tree)
    {
        // std::cerr << pair.first << ":" << pair.second << " ";
        visited[pair.first] = false;
    }

    double cost_cycle = 0;
    if (start != 0)
    {
        cost_cycle += G.distw(0, start);
        visited[0] = true;
    }
    // std::cout << "TSP-i-cost: " << cost_cycle << "\n";
    DFSUtil(G, start, tree, visited, sol, cost_cycle, budget);

    // std::cout << "sol find_TSP: "; print::print_vector_int(sol);

    return sol;
}