#include "graph.h"

graph::graph(int _area_x, int _area_y, int _n_nodes)
{
    this->area_x = _area_x;
    this->area_y = _area_y;
    this->n_nodes = _n_nodes;

    populate_graph();
}

graph::graph() {}

graph::~graph() {}

void graph::populate_graph()
{
    uniform_real_distribution<double> unif_1(0, area_x);
    uniform_real_distribution<double> unif_2(0, area_y);
    default_random_engine re;

    node node_t(0, 0, 0, 0); // v_0 = depot
    vertices.push_back(node_t);

    for (size_t i = 0; i < this->n_nodes; i++)
    {
        double x = unif_1(re); // used of % here is just fine
        double y = unif_2(re);
        node node_t(x, y, 0, 1);
        vertices.push_back(node_t);
    }

    assert(this->vertices.size() == this->n_nodes + 1);
}

double graph::dist(node u, node v)
{
    return sqrt(pow(u.x - v.x, 2) + pow(u.y - v.y, 2));
}

void graph::print_graph()
{
    cout << "***** GRAPH *****" << endl;
    for (size_t i = 0; i < this->vertices.size(); i++)
    {
        cout << i << ": (" << this->vertices[i].x << ", " << this->vertices[i].y << ") p:" << this->vertices[i].priority << " w:" << this->vertices[i].node_weight << endl;
    }
    cout << "*****************" << endl;
}