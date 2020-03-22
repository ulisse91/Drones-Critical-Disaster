#include "top_based.h"

topb::topb(graph _G, int _n_drones, int _n_batteries, double _budget)
{
    this->G = _G;
    this->n_drones = _n_drones;
    this->n_batteries = _n_batteries;
    this->budget = _budget;

    assert(this->budget > 0);
    assert(this->n_batteries >= this->n_drones);
}

topb::~topb() {}

std::vector<std::vector<std::vector<std::pair<int, double>>>> topb::top_path_BB()
{
    std::unordered_set<int> graph_vertices = this->G.get_vertices_set();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol(this->n_drones, std::vector<std::vector<std::pair<int, double>>>());

    graph_vertices.erase(graph_vertices.find(0));

    int current_drone = 0;

    int counter = 0;
    while (not graph_vertices.empty() and counter <= G.get_n_nodes())
    {
        std::unordered_set<int> cycle_set = op_path_BB_insert_step(graph_vertices, {0});

        // print::print_set(cycle_set);

        std::vector<int> cycle_tsp = utilities::set_to_tsp(this->G, this->budget, cycle_set);

        // print::print_vector_int(cycle_tsp);

        if (cycle_tsp.size() != 1)
        {

            std::vector<std::pair<int, double>> _temp;
            for (size_t j = 0; j < cycle_tsp.size(); j++)
            {
                _temp.push_back(std::make_pair(cycle_tsp[j], 1));
            }
            _temp.push_back(std::make_pair(0, 1));
            sol[current_drone].push_back(_temp);

            graph_vertices = utilities::set_difference(graph_vertices, cycle_tsp);

            current_drone = (current_drone + 1) % this->n_drones;
        }

        counter++;
    }
    return sol;
}

std::unordered_set<int> topb::op_path_BB_insert_step(std::unordered_set<int> graph_vertices, std::unordered_set<int> sol_temp)
{
    if (graph_vertices.empty())
    {
        return sol_temp;
    }

    int a = graph_vertices.extract(graph_vertices.begin()).value();
    graph_vertices.erase(a);

    std::unordered_set<int> sol_temp_augmented = sol_temp;
    sol_temp_augmented.insert(a);

    std::unordered_set<int> sol_temp_next_step = op_path_BB_insert_step(graph_vertices, sol_temp);
    std::unordered_set<int> sol_temp_next_step_a = op_path_BB_insert_step(graph_vertices, sol_temp_augmented);

    if (cost_cycle_OP(sol_temp_next_step) > cost_cycle_OP(sol_temp_next_step_a))
    {
        return sol_temp_next_step;
    }
    return sol_temp_next_step_a;
}

double topb::cost_cycle_OP(std::unordered_set<int> _temp)
{
    // print::print_set(_temp);

    std::vector<int> tsp_temp = utilities::set_to_tsp(this->G, this->budget, _temp);

    double cost = utilities::cost_budget_sequence(this->G, tsp_temp);

    // print::print_vector_int(tsp_temp);

    if (cost > this->budget or tsp_temp.size() != _temp.size())
        return -1;

    double sum_of_elems = 0;
    for (auto const &i : tsp_temp)
    {
        sum_of_elems += this->G.get_priority_node(i);
    }

    return sum_of_elems;
}