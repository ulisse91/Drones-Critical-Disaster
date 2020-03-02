#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <vector>
#include <assert.h>
#include <utility>

#include "../src/core/graph.h"
#include "../src/core/simulator.h"
#include "../src/core/algo.h"

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
};

#endif // TEST_H