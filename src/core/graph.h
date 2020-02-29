#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <random>
#include <assert.h>
#include <fstream>
#include <unordered_set>

class graph
{

private:
    struct node
    {
        int id;
        double x, y;
        double node_weight;
        int priority;
        node(int _id, double _x, double _y, double _node_weight, int _priority) : id(_id), x(_x), y(_y), node_weight(_node_weight), priority(_priority) {}
    };

    int area_x;
    int area_y;
    int n_nodes;
    std::vector<graph::node> vertices;

    bool check_double_node(node new_node);

public:
    graph();
    graph(int _area_x, int _area_y);
    ~graph();

    int get_area_x();
    int get_area_y();
    int get_n_nodes();
    double get_weight_node(int id);
    int get_priority_node(int id);
    double get_coord_x(int id);
    double get_coord_y(int id);
    std::vector<int> get_vertices();
    std::unordered_set<int> get_vertices_set();

    double dist(int u, int v);
    void print_graph();
    void create_random_graph();
    int add_node(double _x, double _y, double _node_weight, int _priority);
    int read_graph_from_file(std::string file);
};

#endif // GRAPH_H