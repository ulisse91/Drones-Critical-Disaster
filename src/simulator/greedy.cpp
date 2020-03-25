#include "greedy.h"

greedy::greedy(graph _G, int _n_drones, int _n_batteries, double _budget, std::map<int, int> _sigma_prime_probs, long _seed)
{
    this->G = _G;
    this->n_drones = _n_drones;
    this->n_batteries = _n_batteries;
    this->budget = _budget;
    this->sigma_prime_probs = _sigma_prime_probs;
    this->seed = _seed;

    assert(this->budget > 0);
    assert(this->n_batteries >= this->n_drones);
}

greedy::~greedy() {}

std::vector<int> greedy::greedy_find_path(std::unordered_set<int> graph_vertices, bool max)
{
    std::vector<int> cycle;

    double residual_budget = this->budget;
    int next_step = 0;
    int last_step = 0;

    while (next_step != -1)
    {
        cycle.push_back(next_step);
        if (graph_vertices.find(next_step) != graph_vertices.end())
            graph_vertices.erase(graph_vertices.find(next_step));
        residual_budget -= this->G.distw(last_step, next_step);
        last_step = next_step;
        next_step = -1;
        double min_dist = max ? 0 : this->G.get_n_nodes() * (this->G.get_area_x() + this->G.get_area_y());

        for (auto const &i : graph_vertices)
        {
            if (last_step == i)
                continue;

            if (((max and this->G.get_priority_node(i) / this->G.distw(last_step, i) > min_dist) or (not max and this->G.get_priority_node(i) / this->G.distw(last_step, i) < min_dist)) and this->G.distw(last_step, i) + this->G.distw(0, i) < residual_budget)
            {
                min_dist = this->G.get_priority_node(i) / this->G.distw(last_step, i);
                next_step = i;
            }
        }
    }
    return cycle;
}

std::vector<std::vector<std::vector<std::pair<int, double>>>> greedy::greedy_algorithm(bool max, bool sigma_prime)
{
    std::unordered_set<int> graph_vertices = this->G.get_vertices_set();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol(this->n_drones, std::vector<std::vector<std::pair<int, double>>>());

    graph_vertices.erase(graph_vertices.find(0));

    int current_drone = 0;
    int counter = 0;

    while (not graph_vertices.empty() and counter <= this->G.get_n_nodes())
    {
        std::vector<int> cycle_tsp = greedy_find_path(graph_vertices, max);

        if (sigma_prime)
        {
            cycle_tsp = utilities::check_cycle_sigma_prime_cycle(G, this->sigma_prime_probs, this->seed, budget, cycle_tsp);
        }

        std::vector<std::pair<int, double>> _temp;
        for (auto const &j : cycle_tsp)
        {
            _temp.push_back(std::make_pair(j, 1));
        }
        _temp.push_back(std::make_pair(0, 1));
        sol[current_drone].push_back(_temp);

        graph_vertices = utilities::set_difference(graph_vertices, cycle_tsp);
        current_drone = (current_drone + 1) % this->n_drones;
        counter++;
    }
    return sol;
}