#include "test.h"

test::test() {}
test::~test() {}

///////////////////////////////////////////////////////////////////////
//////////////////////////// GRAPH ////////////////////////////////////
///////////////////////////////////////////////////////////////////////

bool test::check_if_double_node()
{
    /*
        We create 4 nodes in total
        2nd has coordinate y equal to the first one
        3th has coordinate x equal to the first one
        4th is the same node as the first one
    */

    graph G = graph(1, 2);
    G.add_node(0.5, 1, 2, 3, 0);

    if (G.add_node(0, 1, 2, 3, 0) == 1 and G.add_node(0.5, 0.5, 2, 3, 0) == 1 and G.add_node(0.5, 1, 2, 3, 0) == -1)
    {
        std::cout << "check_if_double_node [OK]" << std::endl;
        return true;
    }
    // std::cout << "[ERROR::TEST] check_if_double_node" << std::endl;
    return false;
}

bool test::check_if_node_outside_area()
{
    /*
        We create 3 nodes in total
        first has coordinate x outside area
        second has coordinate y outside area
        third has both coordinates outside area
    */

    graph G = graph(1, 2);

    if (G.add_node(3, 1, 2, 3, 0) == -2 and G.add_node(1, 3, 2, 3, 0) == -2 and G.add_node(3, 4, 2, 3, 0) == -2)
    {
        std::cout << "check_if_node_outside_area [OK]" << std::endl;
        return true;
    }
    // std::cout << "[ERROR::TEST] check_if_node_outside_area" << std::endl;
    return false;
}

bool test::check_read_graph_from_file()
{
    graph G = graph(2, 1);
    if (G.read_graph_from_file("../data/graph/filedoesnotexist.csv") == -1)
    {
        if (G.read_graph_from_file("../data/graph/test_read_graph_from_file.csv") == 1)
        {
            if (G.get_n_nodes() == 5 and G.get_coord_x(1) == 0.5 and G.get_coord_y(1) == 0.8 and G.get_priority_node(1) == 2 and G.get_weight_node(1) == 1.5)
            {
                std::cout << "check_read_graph_from_file [OK]" << std::endl;
                return true;
            }
            return false;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////
/////////////// FEASIBLE SOLUTIONS ////////////////////////////////////
///////////////////////////////////////////////////////////////////////

bool test::check_fsolutions_depot()
{
    graph G = graph(2, 1);
    G.add_node(0.5, 0.8, 1, 1, 0); // a
    G.add_node(1.5, 0.5, 1, 1, 0); // b
    G.add_node(1, 0.2, 1, 1, 0);   // c
    G.add_node(0.7, 0.3, 1, 1, 0); // d

    simulator sim = simulator(G, 1, 1, 60, 0, 0);

    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(4, 1), std::make_pair(0, 1)}}};
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol2 = {{{std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(4, 1), std::make_pair(0, 1)}}};
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol3 = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(4, 1)}}};
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol4 = {{{std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(4, 1)}}};

    if (sim.check_solution_feasible(sol) == 1 and sim.check_solution_feasible(sol2) == -1 and sim.check_solution_feasible(sol3) == -1 and sim.check_solution_feasible(sol4) == -1)
    {
        std::cout << "check_fsolutions_depot [OK]" << std::endl;
        return true;
    }

    return false;
}

bool test::check_fsolutions_node_out_index()
{
    graph G = graph(2, 1);
    G.add_node(0.5, 0.8, 1, 1, 0); // a
    G.add_node(1.5, 0.5, 1, 1, 0); // b
    G.add_node(1, 0.2, 1, 1, 0);   // c
    G.add_node(0.7, 0.3, 1, 1, 0); // d

    simulator sim = simulator(G, 1, 1, 60, 0, 0);

    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(4, 1), std::make_pair(0, 1)}}};
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol2 = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(5, 1), std::make_pair(0, 1)}}};

    if (sim.check_solution_feasible(sol) == 1 and sim.check_solution_feasible(sol2) == -3)
    {
        std::cout << "check_fsolutions_node_out_index [OK]" << std::endl;
        return true;
    }

    return false;
}

bool test::check_fsolutions_cycle_over_budget()
{
    graph G = graph(2, 1);
    G.add_node(0.5, 0.8, 1, 1, 0); // a
    G.add_node(1.5, 0.5, 1, 1, 0); // b
    G.add_node(1, 0.2, 1, 1, 0);   // c
    G.add_node(0.7, 0.3, 1, 1, 0); // d

    simulator sim = simulator(G, 1, 1, 50, 0, 0);
    simulator sim2 = simulator(G, 1, 1, 60, 0, 0);

    // needs B = ~7.65
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(4, 1), std::make_pair(0, 1)}}};

    if (sim.check_solution_feasible(sol) == -2 and sim2.check_solution_feasible(sol) == 1)
    {
        std::cout << "check_fsolutions_cycle_over_budget [OK]" << std::endl;
        return true;
    }

    return false;
}

bool test::check_fsolutions_all_nodes_1()
{
    graph G = graph(2, 1);
    G.add_node(0.5, 0.8, 1, 1, 0); // a
    G.add_node(1.5, 0.5, 1, 1, 0); // b
    G.add_node(1, 0.2, 1, 1, 0);   // c
    G.add_node(0.7, 0.3, 1, 1, 0); // d

    simulator sim = simulator(G, 1, 1, 60, 0, 0);

    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(4, 1), std::make_pair(0, 1)}}};
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol2 = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(0, 1)}}};
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol3 = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(0, 1)}, {std::make_pair(0, 1), std::make_pair(2, 1), std::make_pair(0, 1)}}, {{std::make_pair(0, 1), std::make_pair(3, 1), std::make_pair(0, 1)}}};

    if (sim.check_solution_feasible(sol) == 1 and sim.check_solution_feasible(sol2) == -4 and sim.check_solution_feasible(sol3) == -4)
    {
        std::cout << "check_fsolutions_all_nodes_1 (without portions) [OK]" << std::endl;
        return true;
    }

    return false;
}

bool test::check_fsolutions_all_nodes_2()
{
    graph G = graph(2, 1);
    G.add_node(0.5, 0.8, 1, 1, 0); // a
    G.add_node(1.5, 0.5, 1, 1, 0); // b
    G.add_node(1, 0.2, 1, 1, 0);   // c
    G.add_node(0.7, 0.3, 1, 1, 0); // d

    simulator sim = simulator(G, 1, 1, 8, 0, 0);

    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 0.3), std::make_pair(4, 1), std::make_pair(0, 1)}}, {{std::make_pair(0, 1), std::make_pair(3, 0.7), std::make_pair(0, 1)}}};
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol2 = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(4, 0.5), std::make_pair(0, 1)}}};
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol3 = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(4, 1), std::make_pair(0, 1)}, {std::make_pair(0, 1), std::make_pair(2, 0.2), std::make_pair(0, 1)}}, {{std::make_pair(0, 1), std::make_pair(3, 1), std::make_pair(2, 0.7), std::make_pair(0, 1)}}};

    if (sim.check_solution_feasible(sol) == 1 and sim.check_solution_feasible(sol2) == -4 and sim.check_solution_feasible(sol3) == -4)
    {
        std::cout << "check_fsolutions_all_nodes_2 (with portions) [OK]" << std::endl;
        return true;
    }

    return false;
}

bool test::check_fsolutions_obj_func_value_1()
{
    graph G = graph(2, 1);
    G.add_node(0.5, 0.8, 1, 1, 0); // a
    G.add_node(1.5, 0.5, 1, 1, 0); // b
    G.add_node(1, 0.2, 1, 1, 0);   // c
    G.add_node(0.7, 0.3, 1, 3, 0); // d

    simulator sim = simulator(G, 1, 1, 60, 0, 0);

    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(4, 1), std::make_pair(0, 1)}}};
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol2 = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(0, 1)}, {std::make_pair(0, 1), std::make_pair(4, 1), std::make_pair(0, 1)}}};
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol3 = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(0, 1)}}, {{std::make_pair(0, 1), std::make_pair(4, 1), std::make_pair(0, 1)}}};

    if (sim.check_solution_feasible(sol) == 1 and sim.check_solution_feasible(sol2) == 1 and sim.check_solution_feasible(sol3) == 1)
        if (146 >= sim.evaluate_solution(0, sol) and sim.evaluate_solution(0, sol) >= 145)
            if (135 >= sim.evaluate_solution(0, sol2) and sim.evaluate_solution(0, sol2) >= 134)
                if (92 >= sim.evaluate_solution(0, sol3) and sim.evaluate_solution(0, sol3) >= 91)
                {
                    std::cout << "check_fsolutions_obj_func_value_1 (wL^I) [OK]" << std::endl;
                    return true;
                }
    return false;
}

bool test::check_fsolutions_obj_func_value_2()
{
    graph G = graph(2, 1);
    G.add_node(0.5, 0.8, 1, 1, 0); // a
    G.add_node(1.5, 0.5, 1, 1, 0); // b
    G.add_node(1, 0.2, 1, 1, 0);   // c
    G.add_node(0.7, 0.3, 1, 3, 0); // d

    simulator sim = simulator(G, 1, 1, 60, 0, 0);

    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(4, 1), std::make_pair(0, 1)}}};
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol2 = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(0, 1)}, {std::make_pair(0, 1), std::make_pair(4, 1), std::make_pair(0, 1)}}};
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol3 = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(0, 1)}}, {{std::make_pair(0, 1), std::make_pair(4, 1), std::make_pair(0, 1)}}};

    if (sim.check_solution_feasible(sol) == 1 and sim.check_solution_feasible(sol2) == 1 and sim.check_solution_feasible(sol3) == 1)
        if (46.1 >= sim.evaluate_solution(1, sol) and sim.evaluate_solution(1, sol) >= 46)
            if (59.3 >= sim.evaluate_solution(1, sol2) and sim.evaluate_solution(1, sol2) >= 59)
                if (25.2 >= sim.evaluate_solution(1, sol3) and sim.evaluate_solution(1, sol3) >= 25)
                {
                    std::cout << "check_fsolutions_obj_func_value_2 (wL^II) [OK]" << std::endl;
                    return true;
                }

    return false;
}

bool test::check_fsolutions_obj_func_value_3()
{
    graph G = graph(2, 1);
    G.add_node(0.5, 0.8, 1, 1, 0); // a
    G.add_node(1.5, 0.5, 1, 1, 0); // b
    G.add_node(1, 0.2, 1, 1, 0);   // c
    G.add_node(0.7, 0.3, 1, 3, 0); // d

    simulator sim = simulator(G, 1, 1, 60, 0, 0);

    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(4, 1), std::make_pair(0, 1)}}};
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol2 = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(0, 1)}, {std::make_pair(0, 1), std::make_pair(4, 1), std::make_pair(0, 1)}}};
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol3 = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(0, 1)}}, {{std::make_pair(0, 1), std::make_pair(4, 1), std::make_pair(0, 1)}}};

    if (sim.check_solution_feasible(sol) == 1 and sim.check_solution_feasible(sol2) == 1 and sim.check_solution_feasible(sol3) == 1)
        if (59 >= sim.evaluate_solution(2, sol) and sim.evaluate_solution(2, sol) >= 58.5)
            if (93.2 >= sim.evaluate_solution(2, sol2) and sim.evaluate_solution(2, sol2) >= 93)
                if (57 >= sim.evaluate_solution(2, sol3) and sim.evaluate_solution(2, sol3) >= 56.5)
                {
                    std::cout << "check_fsolutions_obj_func_value_3 (ct) [OK]" << std::endl;
                    return true;
                }

    return false;
}

///////////////////////////////////////////////////////////////////////
/////////////////////// ALGORITHMS ////////////////////////////////////
///////////////////////////////////////////////////////////////////////

bool test::check_primMST()
{
    graph G = graph(2, 1);
    G.read_graph_from_file("../data/graph/test_primMST.csv");
    std::map<int, int> sol = algo::primMST(G, {0}, 100);

    double sum_of_elems = 0;

    std::vector<int> graph_vertices = G.get_vertices();

    for (auto &pair : sol)
    {
        sum_of_elems += G.dist(pair.first, pair.second);
    }

    if (33 >= sum_of_elems and sum_of_elems >= 32.9)
    {
        std::cout << "check_primMST [OK]" << std::endl;
        return true;
    }
    return false;
}

bool test::check_metric_k_center()
{
    graph G = graph(2, 1);
    G.read_graph_from_file("../data/graph/test_primMST.csv");
    int k = 2;
    std::vector<int> sol = algo::metric_k_center(G, k, 0);

    if ((int)sol.size() == k)
    {
        for (size_t i = 0; i < sol.size(); i++)
        {
            if (sol[i] > G.get_n_nodes() or sol[i] < 0)
            {
                return false;
            }
        }
        std::cout << "check_metric_k_center [OK]" << std::endl;
        return true;
    }
    return false;
}

bool test::check_primMST_with_forced_nodes()
{
    graph G = graph(2, 1);
    G.read_graph_from_file("../data/graph/test_primMST.csv");

    std::vector<int> f_nodes = {0, 1, 3};
    std::map<int, int> sol = algo::primMST(G, f_nodes, 10);

    double sum_of_elems = 0;

    for (size_t i = 0; i < sol.size(); i++)
    {
        if (sol[i] != -1)
            sum_of_elems += G.dist(i, sol[i]);
    }

    if (29.5 >= sum_of_elems and sum_of_elems >= 29.4)
    {
        std::cout << "check_primMST_with_forced_nodes [OK]" << std::endl;
        return true;
    }
    return false;
}

bool test::check_primMST_with_forced_nodes_and_budget()
{
    graph G = graph(2, 1);
    G.read_graph_from_file("../data/graph/test_primMST.csv");

    std::vector<int> f_nodes = {0, 1, 3};
    std::map<int, int> sol = algo::primMST(G, f_nodes, 21);

    double sum_of_elems = 0;

    for (size_t i = 0; i < sol.size(); i++)
    {
        if (sol[i] != -1)
            sum_of_elems += G.dist(i, sol[i]);
    }

    if (34.2 >= sum_of_elems and sum_of_elems >= 34)
    {
        std::cout << "check_primMST_with_forced_nodes_and_budget [OK]" << std::endl;
        return true;
    }
    return false;
}

bool test::check_find_TSP()
{
    graph G = graph(2, 1);
    G.read_graph_from_file("../data/graph/test_primMST.csv");
    double budget = 100;
    std::map<int, int> sol = algo::primMST(G, {0}, budget);

    std::vector<int> sol_tsp = algo::find_TSP(G, budget, 0, sol);

    if ((int)sol_tsp.size() == G.get_n_nodes() and sol_tsp[0] == 0 and sol_tsp[1] == 4 and sol_tsp[2] == 1 and sol_tsp[3] == 3 and sol_tsp[4] == 2)
    {
        std::cout << "check_find_TSP [OK]" << std::endl;
        return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////
/////////////////////// SIMULATOR /////////////////////////////////////
///////////////////////////////////////////////////////////////////////

bool test::check_set_to_tsp()
{
    graph G = graph(2, 1);
    G.read_graph_from_file("../data/graph/test_primMST.csv");
    double budget = 100;
    std::map<int, int> sol = algo::primMST(G, {0}, 100);

    std::vector<int> sol_tsp = utilities::set_to_tsp(G, budget, G.get_vertices_set());

    if ((int)sol_tsp.size() == G.get_n_nodes() and sol_tsp[0] == 0 and sol_tsp[1] == 4 and sol_tsp[2] == 1 and sol_tsp[3] == 3 and sol_tsp[4] == 2)
    {
        std::cout << "check_set_to_tsp [OK]" << std::endl;
        return true;
    }
    return false;
}

bool test::check_cost_budget_cycle()
{
    graph G = graph(2, 1);
    G.read_graph_from_file("../data/graph/test_primMST.csv");

    simulator sim = simulator(G, 1, 1, 100, 0, 0);

    std::vector<int> target(G.get_vertices_set().begin(), G.get_vertices_set().end());
    double _value_tsp = utilities::cost_budget_sequence(G, target, sim.sigma_prime_probs);

    if (43.4 > _value_tsp and _value_tsp > 43.2)
    {
        std::cout << "check_cost_budget_cycle [OK]" << std::endl;
        return true;
    }
    return false;
}

bool test::check_cost_cycle_OP()
{
    graph G = graph(2, 1);
    G.read_graph_from_file("../data/graph/test_OP.csv");

    simulator sim = simulator(G, 1, 1, 100, 0, 0);
    double _value_OP = sim.cost_cycle_OP(G.get_vertices_set());

    if (_value_OP == 13)
    {
        std::cout << "check_cost_cycle_OP [OK]" << std::endl;
        return true;
    }
    return false;
}

bool test::check_op_path_BB_insert_step()
{
    graph G = graph(2, 1);

    G.read_graph_from_file("../data/graph/test_OP.csv");
    double budget = 25;
    std::unordered_set<int> graph_vertices = G.get_vertices_set();

    graph_vertices.erase(0);

    simulator sim = simulator(G, 1, 1, budget, 0, 0);
    std::unordered_set<int> cycle_set = sim.op_path_BB_insert_step(graph_vertices, {0});

    std::vector<int> target(cycle_set.begin(), cycle_set.end());
    double _budget_spent_tsp = utilities::cost_budget_sequence(G, target);

    if (budget > _budget_spent_tsp and sim.cost_cycle_OP(cycle_set) == 10)
    {
        std::cout << "check_op_path_BB_insert_step [OK]" << std::endl;
        return true;
    }
    return false;
}

bool test::check_top_path_BB()
{
    graph G = graph(2, 1);
    G.read_graph_from_file("../data/graph/test_primMST.csv");

    double budget = 48;
    int n_drones = 1;
    simulator sim = simulator(G, n_drones, n_drones, budget, 0, 0);
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol = sim.meta_algorithm(7);

    if ((int)sol.size() == n_drones and sol[0].size() == 2 and
        sol[0][0][0].first == 0 and sol[0][0][1].first == 3 and sol[0][0][2].first == 2 and sol[0][0][3].first == 4 and sol[0][0][4].first == 0 and
        sol[0][1][0].first == 0 and sol[0][1][1].first == 1 and sol[0][1][2].first == 0)
    {
        budget = 30;
        n_drones = 1;
        sim = simulator(G, n_drones, n_drones, budget, 0, 0);
        sol = sim.meta_algorithm(7);

        if ((int)sol.size() == n_drones and sol[0].size() == 2 and
            sol[0][0][0].first == 0 and sol[0][0][1].first == 4 and sol[0][0][2].first == 0 and
            sol[0][1][0].first == 0 and sol[0][1][1].first == 1 and sol[0][1][2].first == 0)
        {
            budget = 48;
            n_drones = 2;
            sim = simulator(G, n_drones, n_drones, budget, 0, 0);
            sol = sim.meta_algorithm(7);

            if ((int)sol.size() == n_drones and sol[0].size() == 1 and sol[1].size() == 1 and
                sol[0][0][0].first == 0 and sol[0][0][1].first == 3 and sol[0][0][2].first == 2 and sol[0][0][3].first == 4 and sol[0][0][4].first == 0 and
                sol[1][0][0].first == 0 and sol[1][0][1].first == 1 and sol[1][0][2].first == 0)
            {
                std::cout << "check_top_path_BB [OK]" << std::endl;
                return true;
            }
        }
    }

    return false;
}

bool test::check_feasibility()
{
    graph G = graph(2, 1);
    G.read_graph_from_file("../data/graph/test_primMST.csv");

    double budget = 50;
    int n_drones = 1;
    simulator sim = simulator(G, n_drones, n_drones, budget, 0, 0);

    budget = 45;
    n_drones = 1;
    simulator sim2 = simulator(G, n_drones, n_drones, budget, 0, 0);

    if (sim.check_feasibility() and not sim2.check_feasibility())
    {
        std::cout << "check_feasibility [OK]" << std::endl;
        return true;
    }
    return false;
}

bool test::check_greedy_algorithm()
{
    graph G = graph(2, 1);
    G.read_graph_from_file("../data/graph/test_primMST.csv");

    double budget = 48;
    int n_drones = 1;
    simulator sim = simulator(G, n_drones, n_drones, budget, 0, 0);
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol = sim.meta_algorithm(2);

    if ((int)sol.size() == n_drones and sol[0].size() == 2 and
        sol[0][0][0].first == 0 and sol[0][0][1].first == 4 and sol[0][0][2].first == 3 and sol[0][0][3].first == 1 and sol[0][0][4].first == 0 and
        sol[0][1][0].first == 0 and sol[0][1][1].first == 2 and sol[0][1][2].first == 0)
    {
        std::cout << "check_greedy_algorithm [OK]" << std::endl;
        return true;
    }
    return false;
}

bool test::check_prim_based_alg()
{
    graph G = graph(2, 1);
    G.read_graph_from_file("../data/graph/test_primMST2.csv");

    double budget = 48;
    int n_drones = 1;
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol;

    simulator sim = simulator(G, n_drones, n_drones, budget, 0, 0);

    sol = sim.meta_algorithm(0);
    if ((int)sol.size() == n_drones and sol[0].size() == 3)
    {
        budget = 48;
        n_drones = 2;
        sim = simulator(G, n_drones, n_drones, budget, 0, 0);
        sol = sim.meta_algorithm(0);

        if ((int)sol.size() == n_drones and sol[0].size() == 2)
        {
            std::cout << "check_prim_based_alg [OK]" << std::endl;
            return true;
        }
    }
    return false;
}