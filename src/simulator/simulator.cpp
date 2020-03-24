#include "simulator.h"

simulator::simulator(graph _G, int _n_drones, int _n_batteries, double _budget, double _prob_sigma_prime, long _seed)
{
    this->G = _G;
    this->n_drones = _n_drones;
    this->n_batteries = _n_batteries;
    this->budget = _budget;
    this->seed = _seed;
    this->prob_sigma_prime = _prob_sigma_prime;
    update_sigma_prime();

    assert(this->budget > 0);
    assert(this->n_batteries >= this->n_drones);
}

simulator::~simulator() {}

void simulator::update_sigma_prime()
{
    std::unordered_set<int> graph_vertices = this->G.get_vertices_set();
    std::uniform_real_distribution<double> unif_1(0, 1);
    std::mt19937 re(this->seed);

    for (auto const &i : graph_vertices)
        this->sigma_prime_prob[i] = unif_1(re) < this->prob_sigma_prime ? 1 : 0;
}

///////////////////////////////////////////////////////
/////////////// OBJECTIVE FUNCTIONS ///////////////////
///////////////////////////////////////////////////////

double simulator::objective_function_weighted_latency(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol)
{
    double val_sol = 0;
    for (auto const &drone : sol)
    {
        double previous_time_cycle = 0;
        for (auto const &cycle : drone)
        {
            double cost_nodes_in_cycle = 0;
            for (int nodo = 1; nodo < cycle.size(); nodo++)
            {
                int previous_node_index = cycle[nodo - 1].first;
                int current_node_index = cycle[nodo].first;
                double distance_prev_to_curr_node = this->G.dist(previous_node_index, current_node_index);

                cost_nodes_in_cycle += distance_prev_to_curr_node + G.get_weight_node(current_node_index) + this->sigma_prime_prob[current_node_index] * G.get_weight_prime_node(current_node_index);

                val_sol += G.get_priority_node(current_node_index) * (cost_nodes_in_cycle + previous_time_cycle);
            }
            previous_time_cycle = cost_nodes_in_cycle;
        }
    }
    return val_sol;
}

double simulator::objective_function_cycle(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol)
{
    double val_sol = 0;
    for (auto const &drone : sol)
    {
        double previous_time_cycle = 0;
        for (auto const &cycle : drone)
        {
            double cost_nodes_in_cycle = 0;
            for (int nodo = 1; nodo < cycle.size(); nodo++)
            {
                int previous_node_index = cycle[nodo - 1].first;
                int current_node_index = cycle[nodo].first;
                double distance_prev_to_curr_node = this->G.distw(previous_node_index, current_node_index);

                cost_nodes_in_cycle += distance_prev_to_curr_node + this->sigma_prime_prob[current_node_index] * G.get_weight_prime_node(current_node_index);
            }
            for (auto const &nodo : cycle)
            {
                val_sol += this->G.get_priority_node(nodo.first) * (cost_nodes_in_cycle + previous_time_cycle);
            }
            previous_time_cycle = cost_nodes_in_cycle;
        }
    }
    return val_sol;
}

double simulator::objective_function_completion_time(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol)
{
    double value_fun = 0;
    for (auto const &drone : sol)
    {
        double _temp = 0;
        for (auto const &cycle : drone)
        {
            _temp += utilities::cost_budget_sequence(G, cycle, this->sigma_prime_prob);
        }
        if (_temp > value_fun)
            value_fun = _temp;
    }
    return value_fun;
}

double simulator::evaluate_solution(int which, std::vector<std::vector<std::vector<std::pair<int, double>>>> sol)
{
    double val_sol = -1;
    if (which == 0)
        val_sol = objective_function_cycle(sol);

    if (which == 1)
        val_sol = objective_function_weighted_latency(sol);

    if (which == 2)
        val_sol = objective_function_completion_time(sol);

    return val_sol;
}

///////////////////////////////////////////////////////
///////////////////// UTILITIES ///////////////////////
///////////////////////////////////////////////////////

int simulator::check_solution_feasible(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol)
{
    std::unordered_set<int> nodes = G.get_vertices_set();

    for (auto const &drone : sol)
    {
        for (int cycle = 0; cycle < drone.size(); cycle++)
        {
            if (drone[cycle][0].first != 0 or drone[cycle][drone[cycle].size() - 1].first != 0)
            {
                // std::cout << "[ERROR:simulator]:: first and last node of cycle NOT depot" << std::endl;
                return -1;
            }
            double used_budget = 0;

            for (int nodo = 1; nodo < drone[cycle].size(); nodo++)
            {
                int previous_node_index = drone[cycle][nodo - 1].first;
                int current_node_index = drone[cycle][nodo].first;

                if (previous_node_index >= this->G.get_n_nodes() or current_node_index >= this->G.get_n_nodes())
                {
                    // std::cout << "[ERROR:simulator]:: node-index not valid" << std::endl;
                    return -3;
                }
                double distance_prev_to_curr_node = this->G.distw(previous_node_index, current_node_index);
                used_budget += distance_prev_to_curr_node + this->sigma_prime_prob[current_node_index] * G.get_weight_prime_node(current_node_index);
                nodes.erase(current_node_index);
            }
            if (used_budget > this->budget)
            {
                // std::cout << "[ERROR:simulator]:: drone " << drone << " cycle " << cycle << " not feasible (over budget)!" << std::endl;
                return -2;
            }
        }
    }
    if (nodes.size() > 0)
        return -4;

    return 1;
}

bool simulator::check_feasibility()
{
    for (auto &v : this->G.get_vertices())
        if (2 * G.distw(0, v) > this->budget)
            return false;
    return true;
}

std::vector<std::pair<int, double>> simulator::check_cycle_sigma_prime(std::vector<int> cycle)
{
    std::vector<std::pair<int, double>> effective_cycle = {std::make_pair(0, 1)};

    std::uniform_real_distribution<double> unif_1(0, 1);
    std::mt19937 re(this->seed);

    double cost_cycle = 0;
    for (size_t v = 0; v < cycle.size() - 1; ++v)
    {
        if (cost_cycle + this->G.distw(v, v + 1) + this->sigma_prime_prob[v + 1] * G.get_weight_prime_node(v + 1) + this->G.distw(v + 1, 0) < this->budget)
        {
            // std::cout << this->G.distw(v, v + 1) << " " << coin << " " << G.get_weight_prime_node(v + 1) << " " << this->G.distw(v + 1, 0) << std::endl;
            cost_cycle += this->G.distw(v, v + 1) + this->sigma_prime_prob[v + 1] * G.get_weight_prime_node(v + 1);
            effective_cycle.push_back(std::make_pair(v + 1, 1));
        }
        else
        {
            return effective_cycle;
        }
    }
    return effective_cycle;
}

///////////////////////////////////////////////////////
///////////////////// ALGORITHMS //////////////////////
///////////////////////////////////////////////////////

std::vector<std::vector<std::vector<std::pair<int, double>>>> simulator::prim_based_alg()
{
    primb p = primb(G, this->n_drones, this->n_drones /* batteries */, this->budget);
    return p.prim_based_alg();
}

std::vector<std::vector<std::vector<std::pair<int, double>>>> simulator::top_based_alg()
{
    topb p = topb(G, this->n_drones, this->n_drones /* batteries */, this->budget);
    return p.top_path_BB();
}

std::vector<std::vector<std::vector<std::pair<int, double>>>> simulator::greedy_based_alg(bool max)
{
    greedy p = greedy(G, this->n_drones, this->n_drones /* batteries */, this->budget);
    return p.greedy_algorithm(max);
}

std::vector<std::vector<std::vector<std::pair<int, double>>>> simulator::top_plus_prim()
{
    std::unordered_set<int> graph_vertices = this->G.get_vertices_set();
    graph_vertices.erase(graph_vertices.find(0));
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol;
    for (size_t i = 0; i < this->n_drones; i++)
        sol.push_back(std::vector<std::vector<std::pair<int, double>>>());

    topb p = topb(G, this->n_drones, this->n_drones /* batteries */, this->budget);
    int current_drone = 0;

    for (size_t dr = 0; dr < this->n_drones; dr++)
    {
        std::unordered_set<int> cycle_set = p.op_path_BB_insert_step(graph_vertices, {0});

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
    }

    // print::print_solution(sol);

    graph G_temp = graph(this->G.get_area_x(), this->G.get_area_y());
    for (auto &v : graph_vertices)
    {
        G_temp.add_node(v, this->G.get_coord_x(v), this->G.get_coord_y(v), this->G.get_weight_node(v), this->G.get_priority_node(v), this->G.get_weight_prime_node(v));
    }

    primb pr = primb(G_temp, this->n_drones, this->n_drones /* batteries */, this->budget);
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol_prim_temp = pr.prim_based_alg();

    // print::print_solution(sol_prim_temp);

    for (size_t i = 0; i < this->n_drones; ++i)
    {
        for (size_t j = 0; j < sol_prim_temp[i].size(); ++j)
        {
            sol[i].push_back(sol_prim_temp[i][j]);
        }
    }

    // print::print_solution(sol);

    return sol;
}