#include "simulator.h"

simulator::simulator(graph _G, int _n_drones, int _n_batteries, int _budget)
{
    this->G = _G;
    this->n_drones = _n_drones;
    this->n_batteries = _n_batteries;
    this->budget = _budget;

    assert(this->budget > 0);
    assert(this->n_batteries >= this->n_drones);
}

simulator::~simulator() {}

int simulator::check_solution_feasible(vector<vector<vector<pair<int, double>>>> sol)
{
    vector<double> _temp_nodes(this->G.n_nodes);
    for (size_t i = 0; i < _temp_nodes.size(); i++)
    {
        _temp_nodes[i] = this->G.vertices[i].node_weight;
    }

    for (int drone = 0; drone < sol.size(); drone++)
    {
        for (int cycle = 0; cycle < sol[drone].size(); cycle++)
        {
            if (sol[drone][cycle][0].first != 0 or sol[drone][cycle][sol[drone][cycle].size() - 1].first != 0)
            {
                // cout << "[ERROR:simulator]:: first and last node of cycle NOT depot" << endl;
                return -1;
            }

            double used_budget = 0;
            for (int nodo = 1; nodo < sol[drone][cycle].size(); nodo++)
            {
                int previous_node_index = sol[drone][cycle][nodo - 1].first;
                int current_node_index = sol[drone][cycle][nodo].first;

                if (previous_node_index >= this->G.n_nodes or current_node_index >= this->G.n_nodes)
                {
                    // cout << "[ERROR:simulator]:: node-index not valid" << endl;
                    return -3;
                }

                node u = this->G.vertices[previous_node_index];
                node v = this->G.vertices[current_node_index];
                double distance_prev_to_curr_node = this->G.dist(u, v);

                // cout << "(" << u.x << ", " << u.y << ")<->(" << v.x << ", " << v.y << ") : " << distance_prev_to_curr_node << "\n";

                used_budget += distance_prev_to_curr_node + sol[drone][cycle][nodo].second * v.node_weight;
                _temp_nodes[current_node_index] -= sol[drone][cycle][nodo].second * v.node_weight;
            }
            // cout << used_budget << endl;
            if (used_budget > this->budget)
            {
                // cout << "[ERROR:simulator]:: drone " << drone << " cycle " << cycle << " not feasible (over budget)!" << endl;
                return -2;
            }
        }
    }

    for (size_t i = 0; i < _temp_nodes.size(); i++)
    {
        if (_temp_nodes[i] > 0)
        {
            // cout << this->G.vertices[i].x << ", " << this->G.vertices[i].y << endl;
            return -4;
        }
    }

    return 1;
}

double simulator::objective_function_weighted_latency(vector<vector<vector<pair<int, double>>>> sol)
{
    if (not check_solution_feasible(sol))
        return -1;

    vector<double> _temp_nodes(this->G.n_nodes);
    for (size_t i = 0; i < _temp_nodes.size(); i++)
    {
        _temp_nodes[i] = this->G.vertices[i].node_weight;
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
                node u = this->G.vertices[previous_node_index];
                node v = this->G.vertices[current_node_index];
                double distance_prev_to_curr_node = this->G.dist(u, v);

                cost_nodes_in_cycle += distance_prev_to_curr_node + sol[drone][cycle][nodo].second * v.node_weight;
                _temp_nodes[current_node_index] -= sol[drone][cycle][nodo].second * v.node_weight;

                // cout << current_node_index << ": " << distance_prev_to_curr_node << " " << cost_nodes_in_cycle << " " << previous_time_cycle << " " << sol[drone][cycle][nodo].second << " " << v.node_weight << "\n\n";

                if (_temp_nodes[current_node_index] == 0)
                {
                    // cout << "val: " << v.priority << " " << cost_nodes_in_cycle << " " << previous_time_cycle << "\n";
                    val_sol += v.priority * (cost_nodes_in_cycle + previous_time_cycle);
                }
            }
            previous_time_cycle = cost_nodes_in_cycle;
        }
    }
    return val_sol;
}

double simulator::objective_function_cycle(vector<vector<vector<pair<int, double>>>> sol)
{
    if (not check_solution_feasible(sol))
        return -1;

    vector<double> _temp_nodes(this->G.n_nodes);
    for (size_t i = 0; i < _temp_nodes.size(); i++)
    {
        _temp_nodes[i] = this->G.vertices[i].node_weight;
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
                node u = this->G.vertices[previous_node_index];
                node v = this->G.vertices[current_node_index];
                double distance_prev_to_curr_node = this->G.dist(u, v);

                cost_nodes_in_cycle += distance_prev_to_curr_node + sol[drone][cycle][nodo].second * v.node_weight;
                _temp_nodes[current_node_index] -= sol[drone][cycle][nodo].second * v.node_weight;

                // cout << current_node_index << ": " << distance_prev_to_curr_node << " " << cost_nodes_in_cycle << " " << previous_time_cycle << "\n";
                // cout << cost_nodes_in_cycle << ": " << distance_prev_to_curr_node << ": " << sol[drone][cycle][nodo].second << ": " << v.node_weight << "\n\n";
            }
            for (int nodo = 1; nodo < sol[drone][cycle].size() - 1; nodo++)
            {
                int current_node_index = sol[drone][cycle][nodo].first;
                if (_temp_nodes[current_node_index] == 0)
                {
                    // cout << val_sol << " " << this->G.vertices[current_node_index].priority  << " " <<  cost_nodes_in_cycle  << " " <<  previous_time_cycle << endl;
                    val_sol += this->G.vertices[current_node_index].priority * (cost_nodes_in_cycle + previous_time_cycle);
                }

                // cout << current_node_index << " " << val_sol << "; ";
            }
            previous_time_cycle = cost_nodes_in_cycle;
        }
    }
    return val_sol;
}

double simulator::evaluate_solution(int which, vector<vector<vector<pair<int, double>>>> sol)
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

void simulator::print_solution(vector<vector<vector<pair<int, double>>>> sol)
{
    for (size_t i = 0; i < sol.size(); i++)
    {
        cout << "Drone " << i << endl;
        for (size_t j = 0; j < sol[i].size(); j++)
        {
            cout << j << ": ";
            for (size_t k = 0; k < sol[i][j].size(); k++)
            {
                cout << sol[i][j][k].first << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}