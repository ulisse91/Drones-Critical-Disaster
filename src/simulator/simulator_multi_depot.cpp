#include "simulator_multi_depot.h"

simulator_md::simulator_md(graph _G, std::vector<std::tuple<int, int, double>> _drones, int _n_depots)
{
    this->G = _G;
    this->drones = _drones;
    this->number_of_depots = _n_depots;

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
        {
            return false;
        }
    }
    return true;
}

/////////////////////////////////////////////////////
//////////////////// ALGORITHMS ///////////////////
/////////////////////////////////////////////////////

std::vector<std::vector<int>> simulator_md::greedy_round(std::unordered_set<int> graph_vertices)
{

    std::vector<std::vector<int>> curr_sol;
    std::vector<double> residual_budget;

    for (const auto &i : this->drones)
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

std::vector<std::vector<std::vector<std::pair<int, double>>>> simulator_md::kim_out_loop()
{
    std::unordered_set<int> graph_vertices = this->G.get_vertices_set();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol(this->drones.size(), std::vector<std::vector<std::pair<int, double>>>());

    for (int i = 0; i < this->number_of_depots; i++)
    {
        graph_vertices.erase(graph_vertices.find(i));
    }

    int current_drone = 0;
    int counter = 0;

    while (not graph_vertices.empty() and counter <= 2 * G.get_n_nodes())
    {
        std::vector<std::vector<int>> cycle_tsp = prim_based(graph_vertices);
        utilities::clean_sol(cycle_tsp);

        // std::cout << "cycle_tsp kim outer loop:\n";
        // for (size_t i = 0; i < cycle_tsp.size(); i++)
        // {
        //     for (size_t j = 0; j < cycle_tsp[i].size(); j++)
        //     {
        //         std::cout << cycle_tsp[i][j] << " ";
        //     }
        //     std::cout << std::endl;
        // }

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

            // print::print_set(graph_vertices);
            current_drone = (current_drone + 1) % this->drones.size();

            //
        }

        counter++;
        // if (counter == 12)
        //     assert(0);

        if ((int)graph_vertices.size() == (int)this->number_of_depots)
        {
            bool check_if_only_dummy_nodes = true;
            for (auto i : graph_vertices)
                if (i < this->G.get_n_nodes() - this->number_of_depots)
                    check_if_only_dummy_nodes = false;

            if (check_if_only_dummy_nodes)
                break;
        }
    }

    utilities::clean_sol_full(sol);
    return sol;
}

std::vector<std::vector<std::vector<std::pair<int, double>>>> simulator_md::greedy_out_loop()
{
    std::unordered_set<int> graph_vertices = this->G.get_vertices_set();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol(this->drones.size(), std::vector<std::vector<std::pair<int, double>>>());

    // graph_vertices.erase(graph_vertices.find(0));

    int current_drone = 0;
    int counter = 0;

    // auto start_t2 = std::chrono::high_resolution_clock::now();

    while (not graph_vertices.empty() and counter <= 2 * G.get_n_nodes())
    {
        std::vector<std::vector<int>> cycle_tsp = greedy_round(graph_vertices);
        utilities::clean_sol(cycle_tsp);

        // for (size_t i = 0; i < cycle_tsp.size(); i++)
        // {
        //     for (size_t j = 0; j < cycle_tsp[i].size(); j++)
        //     {
        //         std::cout << cycle_tsp[i][j] << " ";
        //     }
        //     std::cout << std::endl;
        // }

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

            // errore su set difference --> toglie troppe cose: non deve togliere nodo

            graph_vertices = utilities::set_difference(graph_vertices, cycle_tsp[i]);

            // print::print_set(graph_vertices);

            current_drone = (current_drone + 1) % this->drones.size();

            // assert(0);
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
                std::cout << "[ERROR:simulator]:: first and last node of cycle NOT depot" << std::endl;
                return -1;
            }
            double used_budget = 0;

            for (int nodo = 1; nodo < (int)drone[cycle].size(); nodo++)
            {
                int previous_node_index = drone[cycle][nodo - 1].first;
                int current_node_index = drone[cycle][nodo].first;

                if (previous_node_index >= this->G.get_n_nodes() or current_node_index >= this->G.get_n_nodes())
                {
                    std::cout << "[ERROR:simulator]:: node-index not valid" << std::endl;
                    return -3;
                }
                double distance_prev_to_curr_node = this->G.distw(previous_node_index, current_node_index);
                used_budget += distance_prev_to_curr_node;
                nodes.erase(current_node_index);

                // std::cout << "check:: " << previous_node_index << " -> " << current_node_index << " = " << distance_prev_to_curr_node<< std::endl;
            }
            if (used_budget > std::get<2>(this->drones[current_drone]))
            {
                std::cout << "[ERROR:simulator]:: drone " << current_drone << " cycle " << cycle << " not feasible (over budget)!" << std::endl;
                // std::cout << used_budget << " " << std::get<2>(this->drones[current_drone]) << std::endl;
                return -2;
            }
        }
        current_drone++;
    }
    if (nodes.size() > 0)
    {
        for (auto i : nodes)
        {
            if (i >= this->number_of_depots)
            {
                std::cout << "[ERROR:simulator]:: nodes.size() > 0 : " << nodes.size() << std::endl;
                return -4;
            }
        }
    }

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

    // std::cout << "\n\n======================================\n";
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

std::vector<std::vector<int>> simulator_md::prim_based(std::unordered_set<int> graph_vertices)
{

    // std::cout << "prim_based | graph_vertices size: " << graph_vertices.size() << std::endl;

    std::vector<std::vector<int>> curr_sol;
    std::vector<double> residual_budget;

    graph G_temp = graph(this->G.get_area_x(), this->G.get_area_y());

    for (auto const &i : graph_vertices)
    {
        G_temp.add_node_no_checks(i, this->G.get_coord_x(i), this->G.get_coord_y(i), this->G.get_weight_node(i), this->G.get_priority_node(i), this->G.get_weight_prime_node(i));
    }

    // print::print_graph(G_temp);

    for (const auto &i : this->drones)
    {
        curr_sol.push_back({std::get<1>(i)});
        residual_budget.push_back(std::get<2>(i));
    }

    std::map<int, int> tree;
    std::vector<int> centers;

    for (int i = this->G.get_n_nodes() - this->number_of_depots; i < this->G.get_n_nodes(); i++)
    {
        centers.push_back(i);
    }

    // std::cerr << "centers: ";
    // print::print_vector(centers);

    assert((int)centers.size() <= (int)this->number_of_depots);

    // if (this->n_drones > 1)
    //     centers.push_back(0);
    tree = algo::primMST_multi_depot(G_temp, centers, this->drones);

    // std::cerr << "Prim-Tree:\n";
    // print::print_map_int_int(tree);
    // std::cerr << std::endl;

    for (int i = 0; i < (int)this->drones.size(); i++)
    {
        std::vector<int> tsp_i = algo::find_TSP_multi_depot(G_temp, std::get<2>(drones[i]), centers[i], tree);

        // std::cerr << "tsp_i: " << utilities::cost_budget_sequence(G, tsp_i) << " : ";
        // print::print_vector(tsp_i);

        curr_sol[i].insert(curr_sol[i].end(), tsp_i.begin() + 1, tsp_i.end());
    }

    return curr_sol;
}

std::vector<std::vector<std::vector<std::pair<int, double>>>> simulator_md::kim_alternativo()
{
    std::unordered_set<int> graph_vertices = this->G.get_vertices_set();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol(this->drones.size(), std::vector<std::vector<std::pair<int, double>>>());
    int current_drone = 0;
    int counter = 0;

    while (not graph_vertices.empty() and counter <= 2 * G.get_n_nodes())
    {
        std::vector<int> cycle_tsp = prim_based_alternativo(graph_vertices, std::get<1>(this->drones[current_drone]), std::get<2>(this->drones[current_drone]), current_drone);

        std::vector<std::pair<int, double>> _temp;
        for (int j = 0; j < (int)cycle_tsp.size(); j++)
        {
            _temp.push_back(std::make_pair(cycle_tsp[j], 1));
        }
        _temp.push_back(std::make_pair(std::get<1>(this->drones[current_drone]), 1));
        sol[current_drone].push_back(_temp);

        // print::print_solution(sol);

        graph_vertices = utilities::set_difference(graph_vertices, cycle_tsp);

        // std::cout << "Remaining graph_vertices: "; print::print_set(graph_vertices);

        current_drone = (current_drone + 1) % this->drones.size();

        counter++;

        // if (counter == 2)
        // {
        //    assert(0);
        // }
    }

    utilities::clean_sol_full(sol);
    return sol;
}

std::vector<int> simulator_md::prim_based_alternativo(std::unordered_set<int> graph_vertices, int start, double budget, int current_drone)
{

    // std::cout << "prim_based | graph_vertices size: " << graph_vertices.size() << std::endl;

    std::vector<int> curr_sol;
    // int residual_budget;

    graph G_temp = graph(this->G.get_area_x(), this->G.get_area_y(), this->number_of_depots);

    for (auto const &i : graph_vertices)
    {
        G_temp.add_node_no_checks(i, this->G.get_coord_x(i), this->G.get_coord_y(i), this->G.get_weight_node(i), this->G.get_priority_node(i), this->G.get_weight_prime_node(i));
    }

    // print::print_graph(G_temp);

    curr_sol.push_back(start);
    // residual_budget = budget;

    std::map<int, int> tree;

    // if (this->n_drones > 1)
    //     centers.push_back(0);
    tree = algo::primMST_multi_depot_alternativo(G_temp, start, budget, this->number_of_depots);

    // std::cerr << "Prim-Tree:\n";
    // print::print_map_int_int(tree);
    // std::cerr << std::endl;

    // for (int i = 0; i < (int)this->drones.size(); i++)
    // {
    std::vector<int> tsp_i = algo::find_TSP_multi_depot(G_temp, budget, start, tree);

    // std::cerr << "tsp_i: " << utilities::cost_budget_sequence(G, tsp_i) << " : ";
    // print::print_vector(tsp_i);

    curr_sol.insert(curr_sol.end(), tsp_i.begin() + 1, tsp_i.end());
    // }

    return curr_sol;
}