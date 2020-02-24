#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <vector>
#include <assert.h>
#include <utility>

#include "graph.h"
#include "simulator.h"
#include "algo.h"

using namespace std;

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
};

#endif // TEST_H