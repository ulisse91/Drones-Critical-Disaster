#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <vector>
#include <assert.h>
#include <utility>

#include "../src/core/graph.h"
#include "../src/core/algo.h"
#include "../src/simulator/simulator.h"
#include "../src/simulator/utilities.h"

class test
{

private:
public:
    test();
    ~test();

    // graph
    bool check_if_double_node();
    bool check_if_node_outside_area();
    bool check_read_graph_from_file();

    // feasible solutions
    bool check_fsolutions_depot();
    bool check_fsolutions_node_out_index();
    bool check_fsolutions_cycle_over_budget();
    bool check_fsolutions_all_nodes_1();
    bool check_fsolutions_all_nodes_2();
    bool check_fsolutions_obj_func_value_1();
    bool check_fsolutions_obj_func_value_2();

    // algorithms
    bool check_primMST();
    bool check_metric_k_center();
    bool check_primMST_with_forced_nodes();
    bool check_primMST_with_forced_nodes_and_budget();
    bool check_find_TSP();

    // simulator
    bool check_cost_budget_cycle();
    bool check_set_to_tsp();
    bool check_cost_cycle_OP();
    bool check_op_path_BB_insert_step();
    bool check_top_path_BB();
    bool check_feasibility();
    bool check_greedy_algorithm();
    bool check_prim_based_alg();
};

#endif // TEST_H