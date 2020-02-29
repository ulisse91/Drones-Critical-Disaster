#include <iostream>

#include "../src/main.h"
#include "../src/core/graph.h"
#include "../src/core/simulator.h"
#include "../src/core/algo.h"
#include "test.h"

int main(int argc, char **argv)
{

    test T = test();

    // graph
    assert(T.check_if_double_node());
    assert(T.check_if_node_outside_area());
    assert(T.check_read_graph_from_file());

    // feasible solutions
    assert(T.check_fsolutions_depot());
    assert(T.check_fsolutions_node_out_index());
    assert(T.check_fsolutions_cycle_over_budget());
    assert(T.check_fsolutions_all_nodes_1());
    assert(T.check_fsolutions_all_nodes_2());
    assert(T.check_fsolutions_obj_func_value_1());
    assert(T.check_fsolutions_obj_func_value_2());

    // algorithms
    assert(T.check_primMST());
    assert(T.check_metric_k_center());
    assert(T.check_primMST_with_forced_nodes());
    assert(T.check_primMST_with_forced_nodes_and_budget());
    assert(T.check_find_TSP());

    /*
        vanno testati:
        - simulator::op_path_BB_insert_step
        - simulator::top_path_BB
        - simulator::set_to_tsp
        - simulator::cost_cycle_OP
        - simulator::cost_budget_cycle
    */

    return 0;
}