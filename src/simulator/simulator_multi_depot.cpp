#include "simulator_multi_depot.h"

simulator_md::simulator_md(graph _G, std::vector<std::tuple<int, int, double>> _drones)
{
    this->G = _G;
    this->drones = _drones;

    // assert(this->budget > 0);
}

simulator_md::~simulator_md() {}

bool simulator_md::check_feasibility_multi_depot()
{
    for (auto &v : this->G.get_vertices())
    {
        bool temp = false;
        for (std::tuple<int, int, double> drone : this->drones)
        {
            temp = temp or (2 * G.distw(std::get<1>(drone), v) <= std::get<2>(drone));
            // std::cout << temp << ": distw(" << v << ", " << std::get<1>(drone) << ") = " << 2 * G.distw(std::get<1>(drone), v) << " | budget: " << std::get<2>(drone) << "\n";
        }
        if (not temp)
            return false;
    }
    return true;
}

/////////////////////////////////////////////////////
//////////////////// GREEDY ROUND ///////////////////
/////////////////////////////////////////////////////

std::vector<std::vector<int>> simulator_md::greedy_round(std::unordered_set<int> graph_vertices)
{

    std::vector<std::vector<int>> curr_sol;
    std::vector<double> residual_budget;

    for (const auto &i : drones)
    {
        curr_sol.push_back({std::get<1>(i)});
        residual_budget.push_back(std::get<2>(i));
    }

    bool round_finished = false;

    while (!round_finished)
    {
        round_finished = true;
        for (int drone = 0; drone < (int)this->drones.size(); drone++)
        {
            int next_step = -1;
            int last_step = curr_sol[drone][curr_sol[drone].size() - 1];
            double min_dist = 0;
            for (auto const &node : graph_vertices)
            {

                if ((1 / this->G.distw(last_step, node) > min_dist) and this->G.distw(last_step, node) + this->G.distw(curr_sol[drone][0], node) < residual_budget[drone])
                {
                    min_dist = 1 / this->G.distw(last_step, node);
                    next_step = node;
                }
            }
            if (next_step != -1)
            {
                round_finished = false;
                curr_sol[drone].push_back(next_step);
                if (graph_vertices.find(next_step) != graph_vertices.end())
                    graph_vertices.erase(graph_vertices.find(next_step));
                residual_budget[drone] -= this->G.distw(last_step, next_step);
            }
        }
    }
    return curr_sol;
}

std::vector<std::vector<int>> simulator_md::calculate_cycles_round(int which_alg, std::unordered_set<int> graph_vertices)
{
    std::vector<std::vector<int>> cycle;

    switch (which_alg)
    {
    case 6: // GMAX ROUND
        cycle = greedy_round(graph_vertices);
        break;
    default:
        std::cerr << "this should not happen" << std::endl;
        break;
    }

    return cycle;
}

std::vector<std::vector<std::vector<std::pair<int, double>>>> simulator_md::meta_algorithm(int which_alg)
{
    std::unordered_set<int> graph_vertices = this->G.get_vertices_set();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol(this->drones.size(), std::vector<std::vector<std::pair<int, double>>>());

    // graph_vertices.erase(graph_vertices.find(0));

    int current_drone = 0;
    int counter = 0;

    // auto start_t2 = std::chrono::high_resolution_clock::now();

    while (not graph_vertices.empty() and counter <= 2 * G.get_n_nodes())
    {
        std::vector<std::vector<int>> cycle_tsp = calculate_cycles_round(which_alg, graph_vertices);
        utilities::clean_sol(cycle_tsp);

        for (int i = 0; i < (int)cycle_tsp.size(); i++)
        {
            std::vector<std::pair<int, double>> _temp;
            for (int j = 0; j < (int)cycle_tsp[i].size(); j++)
            {
                _temp.push_back(std::make_pair(cycle_tsp[i][j], 1));
            }
            _temp.push_back(std::make_pair(std::get<1>(this->drones[current_drone]), 1));
            sol[current_drone].push_back(_temp);

            // print::print_solution(sol);

            graph_vertices = utilities::set_difference(graph_vertices, cycle_tsp[i]);
            current_drone = (current_drone + 1) % this->drones.size();
        }

        // if (counter % this->n_drones == 0)
        // {
        //     auto stop_t2 = std::chrono::high_resolution_clock::now();
        //     // std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(stop_t2 - start_t2).count() << "\n";
        // }

        counter++;
    }
    utilities::clean_sol_full(sol);
    return sol;
}

int simulator_md::check_solution_feasible(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol)
{
    std::unordered_set<int> nodes = G.get_vertices_set();

    int current_drone = 0;

    for (auto const &drone : sol)
    {
        for (int cycle = 0; cycle < (int)drone.size(); cycle++)
        {
            if (drone[cycle][0].first != std::get<1>(this->drones[current_drone]) or drone[cycle][drone[cycle].size() - 1].first != std::get<1>(this->drones[current_drone]))
            {
                // std::cout << "[ERROR:simulator]:: first and last node of cycle NOT depot" << std::endl;
                return -1;
            }
            double used_budget = 0;

            for (int nodo = 1; nodo < (int)drone[cycle].size(); nodo++)
            {
                int previous_node_index = drone[cycle][nodo - 1].first;
                int current_node_index = drone[cycle][nodo].first;

                if (previous_node_index >= this->G.get_n_nodes() or current_node_index >= this->G.get_n_nodes())
                {
                    // std::cout << "[ERROR:simulator]:: node-index not valid" << std::endl;
                    return -3;
                }
                double distance_prev_to_curr_node = this->G.distw(previous_node_index, current_node_index);
                used_budget += distance_prev_to_curr_node;
                nodes.erase(current_node_index);
            }
            if (used_budget > std::get<2>(this->drones[current_drone]))
            {
                // std::cout << "[ERROR:simulator]:: drone " << drone << " cycle " << cycle << " not feasible (over budget)!" << std::endl;
                return -2;
            }
        }
        current_drone++;
    }
    if (nodes.size() > 0)
        return -4;

    return 1;
}

double simulator_md::objective_function_cycle(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol)
{
    std::unordered_map<int, double> nodes_cost;
    for (auto const &drone : sol)
    {
        double previous_time_cycle = 0;
        for (auto const &cycle : drone)
        {
            double this_cycle = 0;
            for (int nodo = 1; nodo < (int)cycle.size(); nodo++)
            {
                int previous_node_index = cycle[nodo - 1].first;
                int current_node_index = cycle[nodo].first;
                double distance_prev_to_curr_node = this->G.dist(previous_node_index, current_node_index);
                this_cycle += distance_prev_to_curr_node + this->G.get_weight_node(current_node_index);
                nodes_cost[current_node_index] = this_cycle;
            }
            for (auto const &nodo : cycle)
            {
                nodes_cost[nodo.first] += previous_time_cycle + this_cycle;
            }
            previous_time_cycle += this_cycle;
        }
    }

    double val_sol = 0;
    for (auto const &node : nodes_cost)
    {
        val_sol += this->G.get_priority_node(node.first) * node.second;
    }
    return (1.0 / (this->G.get_n_nodes() - 1)) * val_sol;
}

double simulator_md::objective_function_completion_time(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol)
{
    double value_fun = 0;
    for (auto const &drone : sol)
    {
        double _temp = 0;
        for (auto const &cycle : drone)
        {
            // print::print_cycle_sol(cycle);
            _temp += utilities::cost_budget_sequence(this->G, cycle);
            // std::cout << "\n1st: " << _temp << "\n";
        }
        // non mi torna questa parte sotto. Non mi ricordo perche' serviva
        // if (drone.size() > 1)
        // {
        //     std::vector<std::pair<int, double>> args(drone[drone.size() - 1].begin(), drone[drone.size() - 1].end() - 1);

        //     print::print_cycle_sol(args);
        //     _temp += utilities::cost_budget_sequence(this->G, args) + this->G.get_weight_node(drone[drone.size() - 1][drone[drone.size() - 1].size() - 2].first) / 2;

        //     std::cout << "2nd: " << _temp << "\n";
        // }
        if (_temp > value_fun)
            value_fun = _temp;

        // std::cout << "-------------\n";
    }
    return value_fun;
}

double simulator_md::evaluate_solution(int which, std::vector<std::vector<std::vector<std::pair<int, double>>>> sol)
{
    double val_sol = -1;
    if (which == 0)
        val_sol = objective_function_cycle(sol);

    if (which == 2)
        val_sol = objective_function_completion_time(sol);

    return val_sol;
}
