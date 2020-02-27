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

void simulator::print_solution(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol)
{
    for (size_t i = 0; i < sol.size(); i++)
    {
        std::cout << "Drone " << i << std::endl;
        for (size_t j = 0; j < sol[i].size(); j++)
        {
            std::cout << j << ": ";
            for (size_t k = 0; k < sol[i][j].size(); k++)
            {
                std::cout << sol[i][j][k].first << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void simulator::prim_based_alg()
{
    std::cout << "non funzionante -- da completare\n";
    return;
    std::map<int, bool> visited;
    std::vector<int> graph_vertices = this->G.get_vertices();

    visited[0] = true;

    graph G_1 = graph(this->G.get_area_x(), this->G.get_area_y());
    graph G_2 = graph(this->G.get_area_x(), this->G.get_area_y());
    graph G_3 = graph(this->G.get_area_x(), this->G.get_area_y());

    for (size_t i = 0; i < this->G.get_n_nodes(); i++)
    {
        visited[graph_vertices[i]] = false;
        if (G.get_priority_node(graph_vertices[i]) == priority_max)
        {
            G_3.add_node(this->G.get_coord_x(graph_vertices[i]), this->G.get_coord_y(graph_vertices[i]), this->G.get_weight_node(graph_vertices[i]), this->G.get_priority_node(graph_vertices[i]));
        }
        if (G.get_priority_node(graph_vertices[i]) == priority_med)
        {
            G_2.add_node(this->G.get_coord_x(graph_vertices[i]), this->G.get_coord_y(graph_vertices[i]), this->G.get_weight_node(graph_vertices[i]), this->G.get_priority_node(graph_vertices[i]));
        }
        if (G.get_priority_node(graph_vertices[i]) == priority_min)
        {
            G_1.add_node(this->G.get_coord_x(graph_vertices[i]), this->G.get_coord_y(graph_vertices[i]), this->G.get_weight_node(graph_vertices[i]), this->G.get_priority_node(graph_vertices[i]));
        }
    }

    algo alg = algo();

    



    // std::vector<int> centers = alg.metric_k_center(G_3, this->n_drones);
    // centers.push_back({0});
    // std::vector<int> tree = alg.primMST(G_3, centers, this->budget);

    // considerare caso unico drone
    /*
        - recuperare foglie di tree
        - aggiungere le foglie a G_2 e considerarle come centers e far ripartire primMST
        - ripetere per G_1
        - ricostruire soluzione
        - questo è il primo ciclo
    */



    // // n droni > 1
    // for (size_t i = 0; i < this->n_drones; i++)
    // {
    //     if (centers[i] != 0)
    //     {
    //         std::vector<int> cycle = alg.find_TSP(centers[i], tree);
    //     }
    // }

    // creare 3 grafi "uno per priorità"
    // std::vector<int> centers = metric_k_center(G_3, this->n_drones)
    // std::vector<int> tree = primMST(G_3, centers, this->budget)
    // calcola cicli chiamando find_TSP(start, tree) con start = v per ogni nodo v \in centers
    // componi da sopra cicli soluzione primo round
    // calcola valore funzione obj
    // ripeti finchè tutti i nodi non sono visitati

    // questo va esteso a passare a prim anche grafo G_2 e G_1
}

std::vector<int> simulator::op_path_BB_insert_step(std::vector<int> graph_vertices, int i, std::vector<int> sol_temp)
{
    // insert node i in sol_temp
    // calcola TSP -> calcola MST + TSP
    // if \less than this->budget continue
    // ritorna vettore che ha max evaluate_sol_OP( op_path_BB_insert_step(grap_vertices, i+1, sol_temp \cup i),op_path_BB_insert_step(grap_vertices, i+1, sol_temp))
}

std::vector<int> simulator::top_path_BB()
{
    std::vector<int> graph_vertices = this->G.get_vertices();
}