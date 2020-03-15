#include "simulator.h"

simulator::simulator(graph _G, int _n_drones, int _n_batteries, double _budget)
{
    this->G = _G;
    this->n_drones = _n_drones;
    this->n_batteries = _n_batteries;
    this->budget = _budget;

    assert(this->budget > 0);
    assert(this->n_batteries >= this->n_drones);
}

simulator::~simulator() {}

int simulator::check_solution_feasible(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol)
{
    std::vector<int> graph_vertices = G.get_vertices();
    std::vector<double> _temp_nodes(this->G.get_n_nodes());
    for (size_t i = 0; i < _temp_nodes.size(); i++)
    {
        _temp_nodes[i] = G.get_weight_node(graph_vertices[i]);
    }

    for (int drone = 0; drone < sol.size(); drone++)
    {
        for (int cycle = 0; cycle < sol[drone].size(); cycle++)
        {
            if (sol[drone][cycle][0].first != 0 or sol[drone][cycle][sol[drone][cycle].size() - 1].first != 0)
            {
                // std::cout << "[ERROR:simulator]:: first and last node of cycle NOT depot" << std::endl;
                return -1;
            }

            double used_budget = 0;
            for (int nodo = 1; nodo < sol[drone][cycle].size(); nodo++)
            {
                int previous_node_index = sol[drone][cycle][nodo - 1].first;
                int current_node_index = sol[drone][cycle][nodo].first;

                if (previous_node_index >= this->G.get_n_nodes() or current_node_index >= this->G.get_n_nodes())
                {
                    // std::cout << "[ERROR:simulator]:: node-index not valid" << std::endl;
                    return -3;
                }

                double distance_prev_to_curr_node = this->G.dist(previous_node_index, current_node_index);

                // std::cout << "(" << u.x << ", " << u.y << ")<->(" << v.x << ", " << v.y << ") : " << distance_prev_to_curr_node << "\n";

                used_budget += distance_prev_to_curr_node + sol[drone][cycle][nodo].second * G.get_weight_node(current_node_index);
                _temp_nodes[current_node_index] -= sol[drone][cycle][nodo].second * G.get_weight_node(current_node_index);
            }
            // std::cout << used_budget << std::endl;
            if (used_budget > this->budget)
            {
                // std::cout << "[ERROR:simulator]:: drone " << drone << " cycle " << cycle << " not feasible (over budget)!" << std::endl;
                return -2;
            }
        }
    }

    for (size_t i = 0; i < _temp_nodes.size(); i++)
    {
        if (_temp_nodes[i] > 0)
        {
            // std::cout << this->graph_vertices[i].x << ", " << this->graph_vertices[i].y << std::endl;
            return -4;
        }
    }

    return 1;
}

double simulator::objective_function_weighted_latency(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol)
{

    std::vector<int> graph_vertices = G.get_vertices();

    if (not check_solution_feasible(sol))
        return -1;

    std::map<int, double> _temp_nodes;
    for (size_t i = 0; i < graph_vertices.size(); i++)
    {
        _temp_nodes[graph_vertices[i]] = G.get_weight_node(graph_vertices[i]);
    }

    double val_sol = 0;
    for (int drone = 0; drone < sol.size(); drone++)
    {
        double previous_time_cycle = 0;
        for (int cycle = 0; cycle < sol[drone].size(); cycle++)
        {
            double cost_nodes_in_cycle = 0;
            for (int nodo = 1; nodo < sol[drone][cycle].size(); nodo++)
            {
                int previous_node_index = sol[drone][cycle][nodo - 1].first;
                int current_node_index = sol[drone][cycle][nodo].first;
                double distance_prev_to_curr_node = this->G.dist(previous_node_index, current_node_index);

                cost_nodes_in_cycle += distance_prev_to_curr_node + sol[drone][cycle][nodo].second * G.get_weight_node(current_node_index);
                _temp_nodes[current_node_index] -= sol[drone][cycle][nodo].second * G.get_weight_node(current_node_index);

                // std::cout << current_node_index << ": " << distance_prev_to_curr_node << " " << cost_nodes_in_cycle << " " << previous_time_cycle << " " << sol[drone][cycle][nodo].second << " " << v.node_weight << "\n\n";

                if (_temp_nodes[current_node_index] == 0)
                {
                    // std::cout << "val: " << G.get_priority_node(current_node_index) << " " << cost_nodes_in_cycle << " " << previous_time_cycle << "\n";
                    val_sol += G.get_priority_node(current_node_index) * (cost_nodes_in_cycle + previous_time_cycle);
                }
            }
            previous_time_cycle = cost_nodes_in_cycle;
        }
    }
    return val_sol;
}

double simulator::objective_function_cycle(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol)
{
    if (not check_solution_feasible(sol))
        return -1;

    std::map<int, double> _temp_nodes;
    std::vector<int> graph_vertices = this->G.get_vertices();

    for (size_t i = 0; i < graph_vertices.size(); i++)
    {
        _temp_nodes[i] = this->G.get_weight_node(graph_vertices[i]);
    }

    double val_sol = 0;
    for (int drone = 0; drone < sol.size(); drone++)
    {
        double previous_time_cycle = 0;
        for (int cycle = 0; cycle < sol[drone].size(); cycle++)
        {
            double cost_nodes_in_cycle = 0;
            for (int nodo = 1; nodo < sol[drone][cycle].size(); nodo++)
            {
                int previous_node_index = sol[drone][cycle][nodo - 1].first;
                int current_node_index = sol[drone][cycle][nodo].first;
                double distance_prev_to_curr_node = this->G.dist(previous_node_index, current_node_index);

                cost_nodes_in_cycle += distance_prev_to_curr_node + sol[drone][cycle][nodo].second * G.get_weight_node(current_node_index);
                _temp_nodes[current_node_index] -= sol[drone][cycle][nodo].second * G.get_weight_node(current_node_index);

                // std::cout << current_node_index << ": " << distance_prev_to_curr_node << " " << cost_nodes_in_cycle << " " << previous_time_cycle << "\n";
                // std::cout << cost_nodes_in_cycle << ": " << distance_prev_to_curr_node << ": " << sol[drone][cycle][nodo].second << ": " << v.node_weight << "\n\n";
            }
            for (int nodo = 1; nodo < sol[drone][cycle].size() - 1; nodo++)
            {
                int current_node_index = sol[drone][cycle][nodo].first;
                if (_temp_nodes[current_node_index] == 0)
                {
                    // std::cout << val_sol << " " << this->G.vertices[current_node_index].priority  << " " <<  cost_nodes_in_cycle  << " " <<  previous_time_cycle << std::endl;
                    val_sol += this->G.get_priority_node(current_node_index) * (cost_nodes_in_cycle + previous_time_cycle);
                }

                // std::cout << current_node_index << " " << val_sol << "; ";
            }
            previous_time_cycle = cost_nodes_in_cycle;
        }
    }
    return val_sol;
}

double simulator::evaluate_solution(int which, std::vector<std::vector<std::vector<std::pair<int, double>>>> sol)
{
    double val_sol = -1;
    if (which == 0)
    {
        val_sol = objective_function_cycle(sol);
    }
    if (which == 1)
    {
        val_sol = objective_function_weighted_latency(sol);
    }
    return val_sol;
}

bool simulator::check_feasibility()
{
    std::vector<int> graph_vertices = this->G.get_vertices();
    for (int i = 0; i < graph_vertices.size(); i++)
    {
        if (2 * G.distw(0, graph_vertices[i]) > this->budget)
        {
            return false;
        }
    }
    return true;
}

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

std::vector<std::vector<std::vector<std::pair<int, double>>>> simulator::greedy_based_alg()
{
    greedy p = greedy(G, this->n_drones, this->n_drones /* batteries */, this->budget);
    return p.greedy_algorithm();
}
