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
    G.add_node(0.5, 1, 2, 3);

    if (G.add_node(0, 1, 2, 3) == 1 and G.add_node(0.5, 0.5, 2, 3) == 1 and G.add_node(0.5, 1, 2, 3) == -1)
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

    if (G.add_node(3, 1, 2, 3) == -2 and G.add_node(1, 3, 2, 3) == -2 and G.add_node(3, 4, 2, 3) == -2)
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
    if (G.read_graph_from_file("data/graph/filedoesnotexist.csv") == -1)
    {
        if (G.read_graph_from_file("data/graph/test_read_graph_from_file.csv") == 1)
        {
            std::vector<int> _temp = G.get_vertices();
            if (G.get_coord_x(1) == 0.5 and G.get_coord_y(1) == 0.8 and G.get_priority_node(1) == 2 and G.get_weight_node(1) == 1.5)
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
    G.add_node(0.5, 0.8, 1, 1); // a
    G.add_node(1.5, 0.5, 1, 1); // b
    G.add_node(1, 0.2, 1, 1);   // c
    G.add_node(0.7, 0.3, 1, 1); // d

    simulator sim = simulator(G, 1, 1, 10);

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
    G.add_node(0.5, 0.8, 1, 1); // a
    G.add_node(1.5, 0.5, 1, 1); // b
    G.add_node(1, 0.2, 1, 1);   // c
    G.add_node(0.7, 0.3, 1, 1); // d

    simulator sim = simulator(G, 1, 1, 8);

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
    G.add_node(0.5, 0.8, 1, 1); // a
    G.add_node(1.5, 0.5, 1, 1); // b
    G.add_node(1, 0.2, 1, 1);   // c
    G.add_node(0.7, 0.3, 1, 1); // d

    simulator sim = simulator(G, 1, 1, 7);
    simulator sim2 = simulator(G, 1, 1, 8);

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
    G.add_node(0.5, 0.8, 1, 1); // a
    G.add_node(1.5, 0.5, 1, 1); // b
    G.add_node(1, 0.2, 1, 1);   // c
    G.add_node(0.7, 0.3, 1, 1); // d

    simulator sim = simulator(G, 1, 1, 8);

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
    G.add_node(0.5, 0.8, 1, 1); // a
    G.add_node(1.5, 0.5, 1, 1); // b
    G.add_node(1, 0.2, 1, 1);   // c
    G.add_node(0.7, 0.3, 1, 1); // d

    simulator sim = simulator(G, 1, 1, 8);

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
    G.add_node(0.5, 0.8, 1, 1); // a
    G.add_node(1.5, 0.5, 1, 1); // b
    G.add_node(1, 0.2, 1, 1);   // c
    G.add_node(0.7, 0.3, 1, 3); // d

    simulator sim = simulator(G, 1, 1, 8);

    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(4, 1), std::make_pair(0, 1)}}};
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol2 = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(0, 1)}, {std::make_pair(0, 1), std::make_pair(4, 1), std::make_pair(0, 1)}}};
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol3 = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(0, 1)}}, {{std::make_pair(0, 1), std::make_pair(4, 1), std::make_pair(0, 1)}}};
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol4 = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(4, 0.5), std::make_pair(0, 1)}, {std::make_pair(0, 1), std::make_pair(4, 0.5), std::make_pair(0, 1)}}};

    if (sim.check_solution_feasible(sol) == 1 and sim.check_solution_feasible(sol2) == 1 and sim.check_solution_feasible(sol3) == 1 and sim.check_solution_feasible(sol4) == 1)
        if (45.9 >= sim.evaluate_solution(0, sol) and sim.evaluate_solution(0, sol) >= 45.88)
            if (47.1115 >= sim.evaluate_solution(0, sol2) and sim.evaluate_solution(0, sol2) >= 47.1113)
                if (27.3405 >= sim.evaluate_solution(0, sol3) and sim.evaluate_solution(0, sol3) >= 27.3403)
                    if (48.9595 >= sim.evaluate_solution(0, sol4) and sim.evaluate_solution(0, sol4) >= 48.9593)
                    {
                        std::cout << "check_fsolutions_obj_func_value_1 (cycle) [OK]" << std::endl;
                        return true;
                    }

    return false;
}

bool test::check_fsolutions_obj_func_value_2()
{
    graph G = graph(2, 1);
    G.add_node(0.5, 0.8, 1, 1); // a
    G.add_node(1.5, 0.5, 1, 1); // b
    G.add_node(1, 0.2, 1, 1);   // c
    G.add_node(0.7, 0.3, 1, 3); // d

    simulator sim = simulator(G, 1, 1, 8);

    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(4, 1), std::make_pair(0, 1)}}};
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol2 = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(0, 1)}, {std::make_pair(0, 1), std::make_pair(4, 1), std::make_pair(0, 1)}}};
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol3 = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(0, 1)}}, {{std::make_pair(0, 1), std::make_pair(4, 1), std::make_pair(0, 1)}}};
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol4 = {{{std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1), std::make_pair(3, 1), std::make_pair(4, 0.5), std::make_pair(0, 1)}, {std::make_pair(0, 1), std::make_pair(4, 0.5), std::make_pair(0, 1)}}};

    if (sim.check_solution_feasible(sol) == 1 and sim.check_solution_feasible(sol2) == 1 and sim.check_solution_feasible(sol3) == 1 and sim.check_solution_feasible(sol4) == 1)
        if (32.1617 >= sim.evaluate_solution(1, sol) and sim.evaluate_solution(1, sol) >= 32.1615)
            if (36.5572 >= sim.evaluate_solution(1, sol2) and sim.evaluate_solution(1, sol2) >= 36.5570)
                if (16.7862 >= sim.evaluate_solution(1, sol3) and sim.evaluate_solution(1, sol3) >= 16.7860)
                    if (36.7312 >= sim.evaluate_solution(1, sol4) and sim.evaluate_solution(1, sol4) >= 36.7310)
                    {
                        std::cout << "check_fsolutions_obj_func_value_1 (weighted latency) [OK]" << std::endl;
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
    G.read_graph_from_file("data/graph/test_primMST.csv");
    std::vector<int> sol = algo::primMST(G, {0}, 100);

    double sum_of_elems = 0;

    std::vector<int> graph_vertices = G.get_vertices();

    for (size_t i = 0; i < sol.size(); i++)
    {
        sum_of_elems += G.dist(graph_vertices[i], graph_vertices[sol[i]]);
    }

    if (2.1995 >= sum_of_elems and sum_of_elems >= 2.1993)
    {
        std::cout << "check_primMST [OK]" << std::endl;
        return true;
    }
    return false;
}

bool test::check_metric_k_center()
{
    graph G = graph(2, 1);
    G.read_graph_from_file("data/graph/test_primMST.csv");
    int k = 2;
    std::vector<int> sol = algo::metric_k_center(G, k);

    if (sol.size() == k)
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
    G.read_graph_from_file("data/graph/test_primMST.csv");

    std::vector<int> f_nodes = {0, 1, 3};
    std::vector<int> sol = algo::primMST(G, f_nodes, 10);

    double sum_of_elems = 0;

    for (size_t i = 0; i < sol.size(); i++)
    {
        if (sol[i] != -1)
            sum_of_elems += G.dist(i, sol[i]);
    }

    if (3.32635 >= sum_of_elems and sum_of_elems >= 3.32633)
    {
        std::cout << "check_primMST_with_forced_nodes [OK]" << std::endl;
        return true;
    }
    return false;
}

bool test::check_primMST_with_forced_nodes_and_budget()
{
    graph G = graph(2, 1);
    G.read_graph_from_file("data/graph/test_primMST.csv");

    std::vector<int> f_nodes = {0, 1, 3};
    std::vector<int> sol = algo::primMST(G, f_nodes, 1.6);

    double sum_of_elems = 0;

    for (size_t i = 0; i < sol.size(); i++)
    {
        if (sol[i] != -1)
            sum_of_elems += G.dist(i, sol[i]);
    }

    if (2.50173 >= sum_of_elems and sum_of_elems >= 2.50171)
    {
        std::cout << "check_primMST_with_forced_nodes_and_budget [OK]" << std::endl;
        return true;
    }
    return false;
}

bool test::check_find_TSP()
{
    graph G = graph(2, 1);
    G.read_graph_from_file("data/graph/test_primMST.csv");
    std::vector<int> sol = algo::primMST(G, {0}, 100);

    std::vector<int> sol_tsp = algo::find_TSP(0, sol);

    if (sol_tsp.size() == G.get_n_nodes() and sol_tsp[0] == 0 and sol_tsp[1] == 4 and sol_tsp[2] == 1 and sol_tsp[3] == 3 and sol_tsp[4] == 2)
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
    G.read_graph_from_file("data/graph/test_primMST.csv");
    int budget = 100;
    std::vector<int> sol = algo::primMST(G, {0}, 100);

    simulator sim = simulator(G, 1, 1, 100);

    std::vector<int> sol_tsp = sim.set_to_tsp(G.get_vertices_set());

    if (sol_tsp.size() == G.get_n_nodes() and sol_tsp[0] == 0 and sol_tsp[1] == 4 and sol_tsp[2] == 1 and sol_tsp[3] == 3 and sol_tsp[4] == 2)
    {
        std::cout << "check_set_to_tsp [OK]" << std::endl;
        return true;
    }
    return true;
}

bool test::check_cost_budget_cycle()
{
    graph G = graph(2, 1);
    G.read_graph_from_file("data/graph/test_primMST.csv");

    simulator sim = simulator(G, 1, 1, 100);

    double _value_tsp = sim.cost_budget_cycle(G.get_vertices_set());

    if (4.24536 > _value_tsp and _value_tsp > 4.24534)
    {
        std::cout << "check_cost_budget_cycle [OK]" << std::endl;
        return true;
    }
    return false;
}

bool test::check_cost_cycle_OP()
{
    graph G = graph(2, 1);
    G.read_graph_from_file("data/graph/test_primMST.csv");

    simulator sim = simulator(G, 1, 1, 100);
    double _value_OP = sim.cost_cycle_OP(G.get_vertices_set());

    simulator sim2 = simulator(G, 1, 1, 3);
    double _value_OP2 = sim2.cost_cycle_OP(G.get_vertices_set());

    if (_value_OP == 4 and _value_OP2 == 2)
    {
        std::cout << "check_cost_cycle_OP [OK]" << std::endl;
        return true;
    }
    return false;
}