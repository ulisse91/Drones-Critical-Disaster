#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <random>
#include <assert.h>
#include <fstream>
#include <unordered_set>
#include <map>

struct node
{
    int id;
    double x, y;
    double node_weight;
    int priority;
    node(int _id, double _x, double _y, double _node_weight, int _priority) : id(_id), x(_x), y(_y), node_weight(_node_weight), priority(_priority) {}
    node() = default;
};

class graph
{

private:
    int area_x;
    int area_y;
    int n_nodes;
    std::map<int, node> vertices;

    bool check_double_node(double _x, double _y);

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

    double dist(int u, int v, int with_weight);
    double dist(int u, int v);
    double distw(int u, int v);

    int read_graph_from_file(std::string file);

    void create_random_graph(int number_of_nodes, double max_weight, int max_priority);
    void create_random_graph(int number_of_nodes, double max_weight, int max_priority, long seed);
    int add_node(int id, double _x, double _y, double _node_weight, int _priority);
    int add_node(double _x, double _y, double _node_weight, int _priority);
    void erase_graph();
};

#endif // GRAPH_H