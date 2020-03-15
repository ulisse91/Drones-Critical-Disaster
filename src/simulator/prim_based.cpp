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
    while (not graph_vertices.empty() and cycle_counter <= this->G.get_n_nodes())
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
                    previous_used_budget = utilities::cost_budget_sequence(G, sol[i][cycle_counter]);
                }
                else
                {
                    sol[i].push_back({std::make_pair(0, 1)});
                }

                // std::cerr << "prev_budget: " << previous_used_budget << " : "; utilities::print_cycle_sol(sol[i][cycle_counter]);

                // G_temp.print_graph();
                // std::cerr << "center: " << centers[i] << "\n";
                // utilities::print_map_int_int(tree);

                assert(previous_used_budget <= this->budget);
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