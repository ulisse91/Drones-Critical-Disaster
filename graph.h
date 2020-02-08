#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <random>
#include <assert.h> 

using namespace std;

struct node
{
    double x, y;
    int node_weight;
    int priority;
    node(double _x, double _y, int _node_weight, int _priority) : x(_x), y(_y), node_weight(_node_weight), priority(_priority) {}
};

class graph
{

private:
    int area_x;
    int area_y;

    void populate_graph();

public:
    int n_nodes;
    vector<node> vertices;

    graph();
    graph(int _area_x, int _area_y, int _n_nodes);
    ~graph();

    double dist(node u, node v);
    void print_graph();
};

#endif // GRAPH_H