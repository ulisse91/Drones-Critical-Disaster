#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <random>
#include <assert.h>
#include <fstream>

struct node
{
    double x, y;
    double node_weight;
    int priority;
    node(double _x, double _y, double _node_weight, int _priority) : x(_x), y(_y), node_weight(_node_weight), priority(_priority) {}
};

class graph
{

private:
    bool check_double_node(node new_node);

public:
    int area_x;
    int area_y;
    int n_nodes;
    std::vector<node> vertices;

    graph();
    graph(int _area_x, int _area_y);
    ~graph();

    double dist(node u, node v);
    void print_graph();
    void create_random_graph();
    int add_node(double _x, double _y, double _node_weight, int _priority);
    int read_graph_from_file(std::string file);
};

#endif // GRAPH_H