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

bool simulator::check_solution_feasible(vector<vector<vector<pair<int, double>>>> sol)
{
    for (int drone = 0; drone < sol.size(); drone++)
    {
        for (int cycle = 0; cycle < sol[drone].size(); cycle++)
        {
            // check id first and last node is v_0
            assert(sol[drone][cycle][0].first == 0);
            assert(sol[drone][cycle][sol[drone][cycle].size() - 1].first == 0);

            int used_budget = 0;
            for (int nodo = 1; nodo < sol[drone][cycle].size(); nodo++)
            {
                int previous_node_index = sol[drone][cycle][nodo - 1].first;
                int current_node_index = sol[drone][cycle][nodo].first;
                node u = this->G.vertices[previous_node_index];
                node v = this->G.vertices[current_node_index];
                double distance_prev_to_curr_node = this->G.dist(u, v);

                // cout << distance_prev_to_curr_node << " ";

                used_budget += distance_prev_to_curr_node + sol[drone][cycle][nodo].second * v.node_weight;
            }
            if (used_budget > this->budget)
            {
                cout << "[ERROR]:: drone " << drone << " cycle " << cycle << " not feasible (over budget)!" << endl;
                return false;
            }
        }
    }
    return true;
}

double simulator::evaluate_solution(vector<vector<vector<pair<int, double>>>> sol)
{
    if (not check_solution_feasible(sol))
        return -1;

    double val_sol = 0;
    for (int drone = 0; drone < sol.size(); drone++)
    {
        double previous_time = 0;
        for (int cycle = 0; cycle < sol[drone].size(); cycle++)
        {
            for (int nodo = 1; nodo < sol[drone][cycle].size(); nodo++)
            {
                int previous_node_index = sol[drone][cycle][nodo - 1].first;
                int current_node_index = sol[drone][cycle][nodo].first;
                node u = this->G.vertices[previous_node_index];
                node v = this->G.vertices[current_node_index];
                double distance_prev_to_curr_node = this->G.dist(u, v);

                double t_f_node_v = previous_time + distance_prev_to_curr_node + sol[drone][cycle][nodo].second * v.node_weight;

                // cout << current_node_index << " : " << t_f_node_v << " | ";

                previous_time = t_f_node_v;
                val_sol += v.priority * t_f_node_v;
            }
        }
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