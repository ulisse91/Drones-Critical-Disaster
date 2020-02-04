#include "graph.h"

graph::graph(int _area_x, int _area_y, int _n_nodes)
{
    area_x = _area_x;
    area_y = _area_y;
    n_nodes = _n_nodes;

    populate_graph();
}

graph::~graph()
{
}

void graph::populate_graph()
{
    uniform_real_distribution<double> unif_1(0,area_x);
    uniform_real_distribution<double> unif_2(0,area_y);
    default_random_engine re;

    for (size_t i = 0; i < n_nodes; i++)
    {
        double x = unif_1(re); // used of % here is just fine
        double y = unif_2(re);
        node node_t(x,y,0,0);
        vertices.push_back(node_t);
    }
    
}

double graph::dist(node u, node v)
{
    return sqrt(pow(u.x - v.x, 2) + pow(u.y - v.y, 2));
}