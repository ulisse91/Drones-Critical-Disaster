#include "prim_based.h"

primb::primb(graph _G, int _n_drones, int _n_batteries, double _budget)
{
    this->G = _G;
    this->n_drones = _n_drones;
    this->n_batteries = _n_batteries;
    this->budget = _budget;

    assert(this->budget > 0);
    assert(this->n_batteries >= this->n_drones);
}

primb::~primb() {}

std::vector<std::vector<std::vector<std::pair<int, double>>>> primb::prim_based_alg()
{
    std::unordered_set<int> graph_vertices = this->G.get_vertices_set();
    graph_vertices.erase(graph_vertices.find(0));
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol;

    for (size_t i = 0; i < this->n_drones; i++)
        sol.push_back(std::vector<std::vector<std::pair<int, double>>>());

    //////////////////////////////////////////////////////////////////////////////

    graph G_3 = graph(this->G.get_area_x(), this->G.get_area_y());
    graph G_2 = graph(this->G.get_area_x(), this->G.get_area_y());
    graph G_1 = graph(this->G.get_area_x(), this->G.get_area_y());

    int counter = 0;

    // ------------------
    while (not graph_vertices.empty() and counter < 10 /*this->G.get_n_nodes()*/)
    {
        std::map<int, int> tree;
        std::vector<int> centers;
        std::vector<int> centers_g_2;
        std::vector<int> centers_g_1;

        for (auto &i : graph_vertices)
        {
            if (this->G.get_priority_node(i) == priority_max)
            {
                G_3.add_node(i, this->G.get_coord_x(i), this->G.get_coord_y(i), this->G.get_weight_node(i), this->G.get_priority_node(i));
            }
            if (this->G.get_priority_node(i) == priority_med)
            {
                G_2.add_node(i, this->G.get_coord_x(i), this->G.get_coord_y(i), this->G.get_weight_node(i), this->G.get_priority_node(i));
            }
            if (this->G.get_priority_node(i) == priority_min)
            {
                G_1.add_node(i, this->G.get_coord_x(i), this->G.get_coord_y(i), this->G.get_weight_node(i), this->G.get_priority_node(i));
            }
        }

        // print::print_graph(G_3);

        std::vector<std::vector<int>> curr_sol;
        for (size_t dr = 0; dr < this->n_drones; ++dr)
        {
            curr_sol.push_back({0});
        }

        if (G_3.get_n_nodes() > 1)
        {
            std::vector<int> centers = algo::metric_k_center(G_3, this->n_drones);
            // std::cerr << "centers: ";
            // print::print_vector_int(centers);

            assert(centers.size() <= this->n_drones);

            if (this->n_drones > 1)
                centers.push_back(0);
            tree = algo::primMST(G_3, centers, this->budget);
            if (this->n_drones > 1)
                centers.erase(std::remove(centers.begin(), centers.end(), 0), centers.end());

            // std::cerr << "Prim-Tree:\n";
            // print::print_map_int_int(tree);
            // std::cerr << std::endl;

            for (int i = 0; i < centers.size(); i++)
            {
                std::vector<int> tsp_i = algo::find_TSP(G_3, this->budget, centers[i], tree);

                // std::cerr << "tsp_i: " << utilities::cost_budget_sequence(G, tsp_i) << " : ";
                // print::print_vector_int(tsp_i);

                curr_sol[i].insert(curr_sol[i].end(), tsp_i.begin(), tsp_i.end());
                centers_g_2.push_back(tsp_i[tsp_i.size() - 1]);
                graph_vertices = utilities::set_difference(graph_vertices, tsp_i);
            }

            for (auto &v : centers_g_2)
            {
                G_2.add_node(v, this->G.get_coord_x(v), this->G.get_coord_y(v), this->G.get_weight_node(v), this->G.get_priority_node(v));
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

            for (int i = 0; i < centers_g_2.size(); i++)
            {
                double previous_used_budget = utilities::cost_budget_sequence(G, curr_sol[i]);

                assert(previous_used_budget <= this->budget);

                std::vector<int> tsp_i = algo::find_TSP(G_2, this->budget - previous_used_budget, centers_g_2[i], tree);

                // std::cerr << "tsp_i: " << utilities::cost_budget_sequence(G, tsp_i) << " : ";
                // print::print_vector_int(tsp_i);

                curr_sol[i].insert(curr_sol[i].end(), tsp_i.begin(), tsp_i.end());
                centers_g_1.push_back(tsp_i[tsp_i.size() - 1]);
                graph_vertices = utilities::set_difference(graph_vertices, tsp_i);
            }

            clean_sol(curr_sol);

            for (auto &v : centers_g_1)
            {
                G_1.add_node(v, this->G.get_coord_x(v), this->G.get_coord_y(v), this->G.get_weight_node(v), this->G.get_priority_node(v));
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

        for (int i = 0; i < centers_g_1.size(); i++)
        {
            double previous_used_budget = utilities::cost_budget_sequence(G, curr_sol[i]);

            assert(previous_used_budget <= this->budget);

            std::vector<int> tsp_i = algo::find_TSP(G_1, this->budget - previous_used_budget, centers_g_1[i], tree);

            // std::cerr << "tsp_i: " << utilities::cost_budget_sequence(G, tsp_i) << " : ";
            // print::print_vector_int(tsp_i);

            curr_sol[i].insert(curr_sol[i].end(), tsp_i.begin(), tsp_i.end());
            graph_vertices = utilities::set_difference(graph_vertices, tsp_i);
        }

        clean_sol(curr_sol);

        // for (auto &i : curr_sol)
        // {
        //     for (auto &j : i)
        //     {
        //         std::cerr << j << " ";
        //     }
        //     std::cerr << std::endl;
        // }

        for (size_t i = 0; i < curr_sol.size(); ++i)
        {
            sol[i].push_back({std::make_pair(0, 1)});
            for (size_t j = 1; j < curr_sol[i].size(); ++j)
            {
                sol[i][sol[i].size() - 1].push_back(std::make_pair(curr_sol[i][j], 1));
            }
            sol[i][sol[i].size() - 1].push_back(std::make_pair(0, 1));
        }

        G_3.erase_graph();
        G_2.erase_graph();
        G_1.erase_graph();

        counter++;
    }

    return sol;
}

void primb::clean_sol(std::vector<std::vector<int>> &_temp)
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