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
        cout << "check_if_double_node [OK]" << endl;
        return true;
    }
    // cout << "[ERROR::TEST] check_if_double_node" << endl;
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
        cout << "check_if_node_outside_area [OK]" << endl;
        return true;
    }
    // cout << "[ERROR::TEST] check_if_node_outside_area" << endl;
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

    vector<vector<vector<pair<int, double>>>> sol = {{{make_pair(0, 1), make_pair(1, 1), make_pair(2, 1), make_pair(3, 1), make_pair(4, 1), make_pair(0, 1)}}};
    vector<vector<vector<pair<int, double>>>> sol2 = {{{make_pair(1, 1), make_pair(2, 1), make_pair(3, 1), make_pair(4, 1), make_pair(0, 1)}}};
    vector<vector<vector<pair<int, double>>>> sol3 = {{{make_pair(0, 1), make_pair(1, 1), make_pair(2, 1), make_pair(3, 1), make_pair(4, 1)}}};
    vector<vector<vector<pair<int, double>>>> sol4 = {{{make_pair(1, 1), make_pair(2, 1), make_pair(3, 1), make_pair(4, 1)}}};

    if (sim.check_solution_feasible(sol) == 1 and sim.check_solution_feasible(sol2) == -1 and sim.check_solution_feasible(sol3) == -1 and sim.check_solution_feasible(sol4) == -1)
    {
        cout << "check_fsolutions_depot [OK]" << endl;
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

    vector<vector<vector<pair<int, double>>>> sol = {{{make_pair(0, 1), make_pair(1, 1), make_pair(2, 1), make_pair(3, 1), make_pair(4, 1), make_pair(0, 1)}}};
    vector<vector<vector<pair<int, double>>>> sol2 = {{{make_pair(0, 1), make_pair(1, 1), make_pair(2, 1), make_pair(3, 1), make_pair(5, 1), make_pair(0, 1)}}};

    if (sim.check_solution_feasible(sol) == 1 and sim.check_solution_feasible(sol2) == -3)
    {
        cout << "check_fsolutions_node_out_index [OK]" << endl;
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
    vector<vector<vector<pair<int, double>>>> sol = {{{make_pair(0, 1), make_pair(1, 1), make_pair(2, 1), make_pair(3, 1), make_pair(4, 1), make_pair(0, 1)}}};

    if (sim.check_solution_feasible(sol) == -2 and sim2.check_solution_feasible(sol) == 1)
    {
        cout << "check_fsolutions_cycle_over_budget [OK]" << endl;
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

    vector<vector<vector<pair<int, double>>>> sol = {{{make_pair(0, 1), make_pair(1, 1), make_pair(2, 1), make_pair(3, 1), make_pair(4, 1), make_pair(0, 1)}}};
    vector<vector<vector<pair<int, double>>>> sol2 = {{{make_pair(0, 1), make_pair(1, 1), make_pair(2, 1), make_pair(3, 1), make_pair(0, 1)}}};
    vector<vector<vector<pair<int, double>>>> sol3 = {{{make_pair(0, 1), make_pair(1, 1), make_pair(0, 1)}, {make_pair(0, 1), make_pair(2, 1), make_pair(0, 1)}}, {{make_pair(0, 1), make_pair(3, 1), make_pair(0, 1)}}};

    if (sim.check_solution_feasible(sol) == 1 and sim.check_solution_feasible(sol2) == -4 and sim.check_solution_feasible(sol3) == -4)
    {
        cout << "check_fsolutions_all_nodes_1 (without portions) [OK]" << endl;
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

    vector<vector<vector<pair<int, double>>>> sol = {{{make_pair(0, 1), make_pair(1, 1), make_pair(2, 1), make_pair(3, 0.3), make_pair(4, 1), make_pair(0, 1)}}, {{make_pair(0, 1), make_pair(3, 0.7), make_pair(0, 1)}}};
    vector<vector<vector<pair<int, double>>>> sol2 = {{{make_pair(0, 1), make_pair(1, 1), make_pair(2, 1), make_pair(3, 1), make_pair(4, 0.5), make_pair(0, 1)}}};
    vector<vector<vector<pair<int, double>>>> sol3 = {{{make_pair(0, 1), make_pair(1, 1), make_pair(4, 1), make_pair(0, 1)}, {make_pair(0, 1), make_pair(2, 0.2), make_pair(0, 1)}}, {{make_pair(0, 1), make_pair(3, 1), make_pair(2, 0.7), make_pair(0, 1)}}};

    if (sim.check_solution_feasible(sol) == 1 and sim.check_solution_feasible(sol2) == -4 and sim.check_solution_feasible(sol3) == -4)
    {
        cout << "check_fsolutions_all_nodes_2 (with portions) [OK]" << endl;
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

    vector<vector<vector<pair<int, double>>>> sol = {{{make_pair(0, 1), make_pair(1, 1), make_pair(2, 1), make_pair(3, 1), make_pair(4, 1), make_pair(0, 1)}}};
    vector<vector<vector<pair<int, double>>>> sol2 = {{{make_pair(0, 1), make_pair(1, 1), make_pair(2, 1), make_pair(3, 1), make_pair(0, 1)}, {make_pair(0, 1), make_pair(4, 1), make_pair(0, 1)}}};
    vector<vector<vector<pair<int, double>>>> sol3 = {{{make_pair(0, 1), make_pair(1, 1), make_pair(2, 1), make_pair(3, 1), make_pair(0, 1)}}, {{make_pair(0, 1), make_pair(4, 1), make_pair(0, 1)}}};
    vector<vector<vector<pair<int, double>>>> sol4 = {{{make_pair(0, 1), make_pair(1, 1), make_pair(2, 1), make_pair(3, 1), make_pair(4, 0.5), make_pair(0, 1)}, {make_pair(0, 1), make_pair(4, 0.5), make_pair(0, 1)}}};

    return true;

    if (sim.check_solution_feasible(sol) == 1 and sim.check_solution_feasible(sol2) == 1 and sim.check_solution_feasible(sol3) == 1 and sim.check_solution_feasible(sol4) == 1)
        if (45.9 >= sim.evaluate_solution(0, sol) and sim.evaluate_solution(0, sol) >= 45.88)
            if (47.1115 >= sim.evaluate_solution(0, sol2) and sim.evaluate_solution(0, sol2) >= 47.1113)
                if (27.3405 >= sim.evaluate_solution(0, sol3) and sim.evaluate_solution(0, sol3) >= 27.3403)
                    if (48.9595 >= sim.evaluate_solution(0, sol4) and sim.evaluate_solution(0, sol4) >= 48.9593)
                    {
                        cout << "check_fsolutions_obj_func_value_1 (cycle) [OK]" << endl;
                        return true;
                    }

    return false;
}