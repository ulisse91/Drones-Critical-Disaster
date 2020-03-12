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

std::unordered_set<int> simulator::op_path_BB_insert_step(std::unordered_set<int> graph_vertices, std::unordered_set<int> sol_temp)
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

std::vector<std::vector<std::vector<std::pair<int, double>>>> simulator::top_path_BB()
{
    std::unordered_set<int> graph_vertices = this->G.get_vertices_set();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol;

    for (size_t i = 0; i < this->n_drones; i++)
        sol.push_back(std::vector<std::vector<std::pair<int, double>>>());

    graph_vertices.erase(graph_vertices.find(0));

    int current_drone = 0;

    int counter = 0;
    while (not graph_vertices.empty() and counter <= G.get_n_nodes())
    {
        std::unordered_set<int> cycle_set = op_path_BB_insert_step(graph_vertices, {0});
        std::vector<int> cycle_tsp = set_to_tsp(cycle_set);

        std::vector<std::pair<int, double>> _temp;
        for (size_t j = 0; j < cycle_tsp.size(); j++)
        {
            _temp.push_back(std::make_pair(cycle_tsp[j], 1));
        }
        _temp.push_back(std::make_pair(0, 1));
        sol[current_drone].push_back(_temp);

        graph_vertices = utilities::set_difference(graph_vertices, cycle_tsp);

        current_drone = (current_drone + 1) % this->n_drones;

        counter++;
    }
    return sol;
}

std::vector<int> simulator::set_to_tsp(std::unordered_set<int> _temp)
{
    graph G_prime = graph(this->G.get_area_x(), this->G.get_area_y());

    for (const int &i : _temp)
    {
        G_prime.add_node(i, G.get_coord_x(i), G.get_coord_y(i), G.get_weight_node(i), G.get_priority_node(i));
    }

    return algo::find_TSP(G_prime, this->budget, 0, algo::primMST(G_prime, {0}, this->budget));
}

double simulator::cost_cycle_OP(std::unordered_set<int> _temp)
{
    std::vector<int> tsp_temp = set_to_tsp(_temp);

    if (cost_budget_cycle(tsp_temp) == -1 or tsp_temp.size() != _temp.size())
        return -1;

    double sum_of_elems = 0;
    for (size_t i = 0; i < tsp_temp.size(); i++)
    {
        sum_of_elems += this->G.get_priority_node(tsp_temp[i]);
    }

    return sum_of_elems;
}

double simulator::cost_budget_cycle(std::unordered_set<int> _temp)
{
    std::vector<int> tsp_temp = set_to_tsp(_temp);

    double sum_of_elems = 0;

    if (tsp_temp.size() > 1)
    {
        for (size_t i = 0; i < tsp_temp.size() - 1; i++)
        {
            sum_of_elems += this->G.dist(tsp_temp[i], tsp_temp[i + 1]);
        }
        // sum_of_elems += this->G.dist(tsp_temp[tsp_temp.size() - 1], tsp_temp[0]);
    }

    if (sum_of_elems > this->budget)
    {
        return -1;
    }
    return sum_of_elems;
}

double simulator::cost_budget_cycle(std::vector<int> tsp_temp)
{
    double sum_of_elems = 0;

    if (tsp_temp.size() > 1)
    {
        for (size_t i = 0; i < tsp_temp.size() - 1; i++)
        {
            sum_of_elems += this->G.dist(tsp_temp[i], tsp_temp[i + 1]);
            // std::cout << tsp_temp[i]<< " -> " << tsp_temp[i + 1] << " = " << this->G.dist(tsp_temp[i], tsp_temp[i + 1]) << " | " << sum_of_elems << "\n";
        }
        // sum_of_elems += this->G.dist(tsp_temp[tsp_temp.size() - 2], tsp_temp[tsp_temp.size() - 1]);
    }

    if (sum_of_elems > this->budget)
    {
        return -1;
    }
    return sum_of_elems;
}

double simulator::cost_budget_cycle(std::vector<std::pair<int, double>> _temp)
{
    double sum_of_elems = 0;

    if (_temp.size() > 1)
    {
        for (size_t i = 0; i < _temp.size() - 1; i++)
        {
            sum_of_elems += this->G.dist(_temp[i].first, _temp[i + 1].first);
        }
        // sum_of_elems += this->G.dist(_temp[_temp.size() - 1].first, _temp[0].first);
    }
    if (sum_of_elems > this->budget)
    {
        return -1;
    }
    return sum_of_elems;
}

bool simulator::check_feasibility()
{
    std::vector<int> graph_vertices = this->G.get_vertices();
    for (int i = 0; i < graph_vertices.size(); i++)
    {
        if (2 * G.dist(0, graph_vertices[i]) > this->budget)
        {
            return false;
        }
    }
    return true;
}

std::vector<int> simulator::greedy_find_path(std::unordered_set<int> graph_vertices)
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
        residual_budget -= G.dist(last_step, next_step);
        last_step = next_step;
        next_step = -1;
        double min_dist = G.get_area_x() + G.get_area_y();

        for (auto &i : graph_vertices)
        {
            if (last_step == i)
                continue;

            if (G.get_priority_node(i) / G.dist(last_step, i) < min_dist and G.dist(last_step, i) + G.dist(0, i) < residual_budget)
            {
                min_dist = G.get_priority_node(i) / G.dist(0, i);
                next_step = i;
            }
        }
    }
    return cycle;
}

std::vector<std::vector<std::vector<std::pair<int, double>>>> simulator::greedy_algorithm()
{
    std::unordered_set<int> graph_vertices = this->G.get_vertices_set();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol;

    for (size_t i = 0; i < this->n_drones; i++)
        sol.push_back(std::vector<std::vector<std::pair<int, double>>>());

    graph_vertices.erase(graph_vertices.find(0));

    int current_drone = 0;
    int counter = 0;

    while (not graph_vertices.empty() and counter <= G.get_n_nodes())
    {
        std::vector<int> cycle_tsp = greedy_find_path(graph_vertices);

        std::vector<std::pair<int, double>> _temp;
        for (size_t j = 0; j < cycle_tsp.size(); j++)
        {
            _temp.push_back(std::make_pair(cycle_tsp[j], 1));
        }
        _temp.push_back(std::make_pair(0, 1));
        sol[current_drone].push_back(_temp);

        graph_vertices = utilities::set_difference(graph_vertices, cycle_tsp);

        current_drone = (current_drone + 1) % this->n_drones;

        counter++;
    }
    return sol;
}

std::vector<std::vector<std::vector<std::pair<int, double>>>> simulator::prim_based_alg()
{
    std::unordered_set<int> graph_vertices = this->G.get_vertices_set();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol;

    for (size_t i = 0; i < this->n_drones; i++)
        sol.push_back(std::vector<std::vector<std::pair<int, double>>>());
    graph_vertices.erase(graph_vertices.find(0));

    graph G_temp = graph(this->G.get_area_x(), this->G.get_area_y());
    std::vector<int> priorities = {priority_max, priority_med, priority_min};
    std::vector<int> centers;

    int counter = 0;
    int which_g = 0;
    int cycle_counter = 0;
    while (not graph_vertices.empty() and cycle_counter <= 5 /*this->G.get_n_nodes()*/)
    {
        // add centers to current graph
        for (int i = 0; i < centers.size(); i++)
        {
            G_temp.add_node(centers[i], this->G.get_coord_x(centers[i]), this->G.get_coord_y(centers[i]), this->G.get_weight_node(centers[i]), this->G.get_priority_node(centers[i]));
        }
        for (auto &i : graph_vertices)
        {
            if (this->G.get_priority_node(i) == priorities[which_g])
            {
                G_temp.add_node(i, this->G.get_coord_x(i), this->G.get_coord_y(i), this->G.get_weight_node(i), this->G.get_priority_node(i));
            }
        }

        // G_temp.print_graph();

        if (G_temp.get_n_nodes() > 1)
        {
            if (centers.size() == 0)
                centers = algo::metric_k_center(G_temp, this->n_drones);
            // utilities::print_vector_int(centers);
            if (this->n_drones != 1 or (this->n_drones == 1 and which_g != 0 and centers.size() > 1))
                centers.erase(std::remove(centers.begin(), centers.end(), 0), centers.end());

            // utilities::print_vector_int(centers);

            assert(centers.size() <= this->n_drones);

            std::map<int, int> tree = algo::primMST(G_temp, centers, this->budget);
            std::vector<int> new_centers = {0};

            // utilities::print_map_int_int(tree);

            for (int i = 0; i < centers.size(); i++)
            {
                double previous_used_budget = 0;
                if (sol[i].size() > cycle_counter)
                {
                    previous_used_budget = cost_budget_cycle(sol[i][cycle_counter]);
                }
                else
                {
                    sol[i].push_back({std::make_pair(0, 1)});
                }

                // std::cerr << "prev_budget: " << previous_used_budget << " : "; utilities::print_cycle_sol(sol[i][cycle_counter]);

                // G_temp.print_graph();
                // std::cerr << "center: " << centers[i] << "\n";
                // utilities::print_map_int_int(tree);

                assert(previous_used_budget != -1);
                std::vector<int> tsp_i = algo::find_TSP(G_temp, this->budget - previous_used_budget, centers[i], tree);

                // std::cerr << "tsp_i: " << cost_budget_cycle(tsp_i) << " : "; utilities::print_vector_int(tsp_i);

                for (size_t j = 0; j < tsp_i.size(); j++)
                {
                    if (sol[i][cycle_counter][sol[i][cycle_counter].size() - 1].first == tsp_i[j])
                        continue;
                    sol[i][cycle_counter].push_back(std::make_pair(tsp_i[j], 1));
                }
                new_centers.push_back(tsp_i[tsp_i.size() - 1]);
                graph_vertices = utilities::set_difference(graph_vertices, tsp_i);
            }
            centers = new_centers;
        }

        // utilities::print_solution(sol);

        G_temp.erase_graph();
        if (which_g == 2)
        {
            centers.clear();
            cycle_counter++;
        }
        which_g = (which_g + 1) % 3; // 3 = #different priorities
        counter++;
    }

    for (size_t i = 0; i < sol.size(); i++)
    {
        for (size_t j = 0; j < sol[i].size(); j++)
        {
            if (sol[i][j].size() == 1)
            {
                sol[i][j] = std::vector<std::pair<int, double>>();
            }
            else
            {
                sol[i][j].push_back({std::make_pair(0, 1)});
            }
        }
    }

    return sol;
}