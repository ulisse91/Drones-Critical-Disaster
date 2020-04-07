#include "simulator.h"

#include <chrono>

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
        this->sigma_prime_probs[i] = unif_1(re) < this->prob_sigma_prime ? 1 : 0;
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
            for (int nodo = 1; nodo < (int)cycle.size(); nodo++)
            {
                int previous_node_index = cycle[nodo - 1].first;
                int current_node_index = cycle[nodo].first;
                double distance_prev_to_curr_node = this->G.dist(previous_node_index, current_node_index);

                cost_nodes_in_cycle += distance_prev_to_curr_node + G.get_weight_node(current_node_index) + this->sigma_prime_probs[current_node_index] * G.get_weight_prime_node(current_node_index);

                val_sol += G.get_priority_node(current_node_index) * (cost_nodes_in_cycle + previous_time_cycle);
            }
            previous_time_cycle = cost_nodes_in_cycle;
        }
    }
    return (1.0 / this->G.get_n_nodes()) * val_sol;
}

double simulator::objective_function_cycle(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol)
{
    std::unordered_map<int, double> nodes_cost;

    for (auto const &drone : sol)
    {
        double previous_time_cycle = 0;
        for (auto const &cycle : drone)
        {
            for (int nodo = 1; nodo < (int)cycle.size(); nodo++)
            {
                int previous_node_index = cycle[nodo - 1].first;
                int current_node_index = cycle[nodo].first;
                double distance_prev_to_curr_node = this->G.dist(previous_node_index, current_node_index);
                previous_time_cycle += distance_prev_to_curr_node + this->G.get_weight_node(current_node_index) + this->sigma_prime_probs[current_node_index] * G.get_weight_prime_node(current_node_index);
                nodes_cost[current_node_index] = previous_time_cycle;
            }
            for (auto const &nodo : cycle)
            {
                nodes_cost[nodo.first] += previous_time_cycle;
            }
        }
    }

    double val_sol = 0;
    for (auto const &node : nodes_cost)
    {
        val_sol += this->G.get_priority_node(node.first) * node.second;
    }

    return (1.0 / this->G.get_n_nodes()) * val_sol;
}

double simulator::objective_function_completion_time(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol)
{
    double value_fun = 0;
    for (auto const &drone : sol)
    {
        double _temp = 0;
        for (auto const &cycle : drone)
        {
            _temp += utilities::cost_budget_sequence(G, cycle, this->sigma_prime_probs);
        }
        if (_temp > value_fun)
            value_fun = _temp;
    }
    return value_fun;
}

std::vector<double> simulator::completion_time_priorities(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol)
{
    std::vector<double> value_fun = {0, 0, 0};

    for (auto const &drone : sol)
    {
        double previous_time_cycle = 0;
        for (auto const &cycle : drone)
        {
            double cost_nodes_in_cycle = 0;
            for (int nodo = 1; nodo < (int)cycle.size(); nodo++)
            {
                int previous_node_index = cycle[nodo - 1].first;
                int current_node_index = cycle[nodo].first;
                double distance_prev_to_curr_node = this->G.dist(previous_node_index, current_node_index);

                cost_nodes_in_cycle += distance_prev_to_curr_node + G.get_weight_node(current_node_index) + this->sigma_prime_probs[current_node_index] * G.get_weight_prime_node(current_node_index);

                if (this->G.get_priority_node(current_node_index) == this->priority_max and value_fun[0] < cost_nodes_in_cycle + previous_time_cycle)
                {
                    value_fun[0] = cost_nodes_in_cycle + previous_time_cycle;
                }
                else if (this->G.get_priority_node(current_node_index) == this->priority_med and value_fun[1] < cost_nodes_in_cycle + previous_time_cycle)
                {
                    value_fun[1] = cost_nodes_in_cycle + previous_time_cycle;
                }
                else if (this->G.get_priority_node(current_node_index) == this->priority_min and value_fun[2] < cost_nodes_in_cycle + previous_time_cycle)
                {
                    value_fun[2] = cost_nodes_in_cycle + previous_time_cycle;
                }
            }
            previous_time_cycle = cost_nodes_in_cycle;
        }
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
        for (int cycle = 0; cycle < (int)drone.size(); cycle++)
        {
            if (drone[cycle][0].first != 0 or drone[cycle][drone[cycle].size() - 1].first != 0)
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
                used_budget += distance_prev_to_curr_node + this->sigma_prime_probs[current_node_index] * G.get_weight_prime_node(current_node_index);
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
        if (2 * G.distw(0, v) + this->sigma_prime_probs[v] * G.get_weight_prime_node(v) > this->budget)
            return false;
    return true;
}

/////////////////////////////////////////////////////
///////////////////// TOP 2 /////////////////////////
/////////////////////////////////////////////////////

std::vector<std::vector<int>> simulator::top_heur(std::unordered_set<int> graph_vertices)
{
    std::vector<std::vector<int>> curr_sol(this->n_drones, {0});
    std::map<int, int> tree;
    graph temp_graph = graph(2, 2);

    for (auto const &i : graph_vertices)
    {
        temp_graph.add_node(i, this->G.get_coord_x(i), this->G.get_coord_y(i), this->G.get_weight_node(i), this->G.get_priority_node(i), this->G.get_weight_prime_node(i));
    }

    std::vector<int> centers = algo::metric_k_center(temp_graph, this->n_drones);
    if (this->n_drones > 1)
        centers.push_back(0);
    tree = algo::primMST(temp_graph, centers, this->budget);
    if (this->n_drones > 1)
        centers.erase(std::remove(centers.begin(), centers.end(), 0), centers.end());

    for (int i = 0; i < (int)centers.size(); i++)
    {
        std::vector<int> tsp_i = algo::find_TSP(temp_graph, this->budget, centers[i], tree);
        curr_sol[i].insert(curr_sol[i].end(), tsp_i.begin(), tsp_i.end());
        curr_sol[i].push_back(0);
        graph_vertices = utilities::set_difference(graph_vertices, curr_sol[i]);
    }

    for (int i = 0; i < (int)centers.size(); i++)
    {
        for (int j = 1; j < (int)curr_sol[i].size() - 1; j++)
        {
            double prev_cost = cost_cycle_OP(curr_sol[i]);
            int new_node = curr_sol[i][j];
            for (auto const &node : graph_vertices)
            {
                curr_sol[i][j] = node;

                if (utilities::cost_budget_sequence(this->G, curr_sol[i]) < this->budget and cost_cycle_OP(curr_sol[i]) > prev_cost)
                {
                    //     print::print_vector(curr_sol[i]);
                    // std::cout << utilities::cost_budget_sequence(this->G, curr_sol[i]) << "\n";
                    new_node = node;
                }
            }
            curr_sol[i][j] = new_node;
            graph_vertices = utilities::set_difference(graph_vertices, {new_node});
        }
    }
    //clean_sol(curr_sol);
    return curr_sol;
}

/////////////////////////////////////////////////////
///////////////////// TOP ///////////////////////////
/////////////////////////////////////////////////////

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

double simulator::cost_cycle_OP(std::unordered_set<int> _temp)
{
    // print::print_set(_temp);

    std::vector<int> tsp_temp = utilities::set_to_tsp(this->G, this->budget, _temp);

    double cost = utilities::cost_budget_sequence(this->G, tsp_temp, this->sigma_prime_probs);

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

double simulator::cost_cycle_OP(std::vector<int> _temp)
{
    // print::print_set(_temp);

    double cost = utilities::cost_budget_sequence(this->G, _temp, this->sigma_prime_probs);

    // print::print_vector_int(tsp_temp);

    if (cost > this->budget or _temp.size() != _temp.size())
        return -1;

    double sum_of_elems = 0;
    for (auto const &i : _temp)
    {
        sum_of_elems += this->G.get_priority_node(i);
    }

    return sum_of_elems;
}

/////////////////////////////////////////////////////
//////////////////// GREEDY /////////////////////////
/////////////////////////////////////////////////////

std::vector<int> simulator::greedy_find_path(std::unordered_set<int> graph_vertices, bool max)
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

/////////////////////////////////////////////////////
///////////////////// PRIM ///////////////////////////
/////////////////////////////////////////////////////

std::vector<std::vector<int>> simulator::prim_based(std::unordered_set<int> graph_vertices)
{
    std::vector<std::vector<int>> curr_sol(this->n_drones, {0});

    std::map<int, int> tree;
    std::vector<int> centers;
    std::vector<int> centers_g_2;
    std::vector<int> centers_g_1;

    graph G_3 = graph(this->G.get_area_x(), this->G.get_area_y());
    graph G_2 = graph(this->G.get_area_x(), this->G.get_area_y());
    graph G_1 = graph(this->G.get_area_x(), this->G.get_area_y());

    for (auto const &i : graph_vertices)
    {
        if (this->G.get_priority_node(i) == priority_max)
        {
            G_3.add_node(i, this->G.get_coord_x(i), this->G.get_coord_y(i), this->G.get_weight_node(i), this->G.get_priority_node(i), this->G.get_weight_prime_node(i));
        }
        if (this->G.get_priority_node(i) == priority_med)
        {
            G_2.add_node(i, this->G.get_coord_x(i), this->G.get_coord_y(i), this->G.get_weight_node(i), this->G.get_priority_node(i), this->G.get_weight_prime_node(i));
        }
        if (this->G.get_priority_node(i) == priority_min)
        {
            G_1.add_node(i, this->G.get_coord_x(i), this->G.get_coord_y(i), this->G.get_weight_node(i), this->G.get_priority_node(i), this->G.get_weight_prime_node(i));
        }
    }

    if (G_3.get_n_nodes() > 1)
    {
        centers = algo::metric_k_center(G_3, this->n_drones);
        // std::cerr << "centers: ";
        // print::print_vector(centers);

        assert((int)centers.size() <= this->n_drones);

        if (this->n_drones > 1)
            centers.push_back(0);
        tree = algo::primMST(G_3, centers, this->budget);
        if (this->n_drones > 1)
            centers.erase(std::remove(centers.begin(), centers.end(), 0), centers.end());

        // std::cerr << "Prim-Tree:\n";
        // print::print_map_int_int(tree);
        // std::cerr << std::endl;

        for (int i = 0; i < (int)centers.size(); i++)
        {
            std::vector<int> tsp_i = algo::find_TSP(G_3, this->budget, centers[i], tree);

            // std::cerr << "tsp_i: " << utilities::cost_budget_sequence(G, tsp_i, sigma_prime_probs) << " : ";
            // print::print_vector(tsp_i);

            curr_sol[i].insert(curr_sol[i].end(), tsp_i.begin(), tsp_i.end());
            centers_g_2.push_back(tsp_i[tsp_i.size() - 1]);
        }

        for (auto const &v : centers_g_2)
        {
            G_2.add_node(v, this->G.get_coord_x(v), this->G.get_coord_y(v), this->G.get_weight_node(v), this->G.get_priority_node(v), this->G.get_weight_prime_node(v));
        }
    }
    else
    {
        centers_g_2 = algo::metric_k_center(G_2, this->n_drones);
    }

    // print::print_graph(G_2);

    if (G_2.get_n_nodes() > 1)

    {
        if (this->n_drones > 1)
            centers_g_2.push_back(0);
        tree = algo::primMST(G_2, centers_g_2, this->budget);
        if (this->n_drones > 1)
            centers_g_2.erase(std::remove(centers_g_2.begin(), centers_g_2.end(), 0), centers_g_2.end());

        // std::cerr << "Prim-Tree:\n";
        // print::print_map_int_int(tree);
        // std::cerr << std::endl;

        for (int i = 0; i < (int)centers_g_2.size(); i++)
        {
            double previous_used_budget = utilities::cost_budget_sequence(G, curr_sol[i]);

            // assert(previous_used_budget <= this->budget);

            std::vector<int> tsp_i = algo::find_TSP(G_2, this->budget - previous_used_budget, centers_g_2[i], tree);

            // std::cerr << "tsp_i: " << utilities::cost_budget_sequence(G, tsp_i, sigma_prime_probs) << " : ";
            // print::print_vector(tsp_i);

            curr_sol[i].insert(curr_sol[i].end(), tsp_i.begin(), tsp_i.end());
            centers_g_1.push_back(tsp_i[tsp_i.size() - 1]);
        }

        for (auto const &v : centers_g_1)
        {
            G_1.add_node(v, this->G.get_coord_x(v), this->G.get_coord_y(v), this->G.get_weight_node(v), this->G.get_priority_node(v), this->G.get_weight_prime_node(v));
        }
    }
    else
    {
        centers_g_1 = algo::metric_k_center(G_1, this->n_drones);
    }

    // print::print_graph(G_1);

    if (this->n_drones > 1)
        centers_g_1.push_back(0);
    tree = algo::primMST(G_1, centers_g_1, this->budget);
    if (this->n_drones > 1)
        centers_g_1.erase(std::remove(centers_g_1.begin(), centers_g_1.end(), 0), centers_g_1.end());

    // std::cerr << "Prim-Tree:\n";
    // print::print_map_int_int(tree);
    // std::cerr << std::endl;

    for (int i = 0; i < (int)centers_g_1.size(); i++)
    {
        double previous_used_budget = utilities::cost_budget_sequence(G, curr_sol[i]);

        // assert(previous_used_budget <= this->budget);

        std::vector<int> tsp_i = algo::find_TSP(G_1, this->budget - previous_used_budget, centers_g_1[i], tree);

        // std::cerr << "tsp_i: " << utilities::cost_budget_sequence(G, tsp_i, sigma_prime_probs) << " : ";
        //     print::print_vector(tsp_i);

        curr_sol[i].insert(curr_sol[i].end(), tsp_i.begin(), tsp_i.end());
    }

    clean_sol(curr_sol);

    return curr_sol;
}

void simulator::clean_sol(std::vector<std::vector<int>> &_temp)
{
    for (auto &cycle : _temp)
    {
        auto end = cycle.end();
        for (auto it = cycle.begin(); it != end; ++it)
        {
            end = std::remove(it + 1, end, *it);
        }
        cycle.erase(end, cycle.end());
    }
}

/////////////////////////////////////////////////////
//////////////////// ALGORITHM //////////////////////
/////////////////////////////////////////////////////

std::vector<std::vector<int>> simulator::calculate_cycles_round(int which_alg, std::unordered_set<int> graph_vertices)
{
    std::vector<std::vector<int>> cycle;

    switch (which_alg)
    {
    case 0:
        cycle = prim_based(graph_vertices);
        break;
    case 1:
        cycle = top_heur(graph_vertices);
        break;
    case 2:
        cycle.push_back(greedy_find_path(graph_vertices, true));
        break;
    case 3:
        cycle.push_back(greedy_find_path(graph_vertices, false));
        break;
    case 4:
        cycle = calculate_cycles_round(1, graph_vertices);
        break;
    case 5:
        cycle.push_back(utilities::set_to_tsp(this->G, this->budget, op_path_BB_insert_step(graph_vertices, {0})));
        break;
    default:
        std::cerr << "this should'nt happen" << std::endl;
        break;
    }

    return cycle;
}

std::vector<int> simulator::check_cycle_sigma_prime_cycle(std::vector<int> cycle)
{
    // print::print_vector_int(cycle);

    std::vector<int> effective_cycle = {};
    if (cycle.size() > 0)
        effective_cycle = {cycle[0]};

    double cost_cycle = 0;
    for (int v = 0; v < ((int)cycle.size()) - 1; v++)
    {
        // std::cerr << cost_cycle << " " << G.distw(v, v + 1) << " " << sigma_prime_prob[v + 1] * G.get_weight_prime_node(v + 1) << " " << G.distw(v + 1, 0) << std::endl;

        if (cost_cycle + this->G.distw(cycle[v], cycle[v + 1]) + this->sigma_prime_probs[cycle[v + 1]] * this->G.get_weight_prime_node(cycle[v + 1]) + this->G.distw(cycle[v + 1], 0) < this->budget)
        {
            cost_cycle += this->G.distw(cycle[v], cycle[v + 1]) + this->sigma_prime_probs[cycle[v + 1]] * this->G.get_weight_prime_node(cycle[v + 1]);
            effective_cycle.push_back(cycle[v + 1]);
        }
        else
        {
            // print::print_vector_int(effective_cycle);
            return effective_cycle;
        }
    }
    // print::print_vector_int(effective_cycle);
    return effective_cycle;
}

std::vector<std::vector<std::vector<std::pair<int, double>>>> simulator::meta_algorithm(int which_alg)
{
    std::unordered_set<int> graph_vertices = this->G.get_vertices_set();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol(this->n_drones, std::vector<std::vector<std::pair<int, double>>>());

    graph_vertices.erase(graph_vertices.find(0));

    int current_drone = 0;
    int counter = 0;

    // auto start_t2 = std::chrono::high_resolution_clock::now();

    while (not graph_vertices.empty() and counter <= G.get_n_nodes())
    {
        std::vector<std::vector<int>> cycle_tsp = calculate_cycles_round(which_alg, graph_vertices);

        if (this->prob_sigma_prime > 0)
        {
            for (int i = 0; i < (int)cycle_tsp.size(); i++)
            {
                cycle_tsp[i] = check_cycle_sigma_prime_cycle(cycle_tsp[i]);
            }
        }

        for (int i = 0; i < (int)cycle_tsp.size(); i++)
        {
            std::vector<std::pair<int, double>> _temp;
            for (int j = 0; j < (int)cycle_tsp[i].size(); j++)
            {
                _temp.push_back(std::make_pair(cycle_tsp[i][j], 1));
            }
            _temp.push_back(std::make_pair(0, 1));
            sol[current_drone].push_back(_temp);
            graph_vertices = utilities::set_difference(graph_vertices, cycle_tsp[i]);
            current_drone = (current_drone + 1) % this->n_drones;
        }

        if (which_alg == 4)
        {
            which_alg = 0;
        }

        // if (counter % this->n_drones == 0)
        // {
        //     auto stop_t2 = std::chrono::high_resolution_clock::now();
        //     // std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(stop_t2 - start_t2).count() << "\n";
        // }

        counter++;
    }

    return sol;
}