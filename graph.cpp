#include "graph.h"

graph::graph(int _area_x, int _area_y)
{
    this->area_x = _area_x;
    this->area_y = _area_y;

    node depot(0, 0, 0, 0); // v_0 = depot
    this->vertices.push_back(depot);

    this->n_nodes = 1;
}

graph::graph() {}

graph::~graph() {}

void graph::create_random_graph()
{
    cout << "TO_DO\n";
    // // da fare
    // uniform_real_distribution<double> unif_1(0, area_x);
    // uniform_real_distribution<double> unif_2(0, area_y);
    // default_random_engine re;

    // node node_t(0, 0, 0, 0); // v_0 = depot
    // vertices.push_back(node_t);

    // for (size_t i = 0; i < this->n_nodes; i++)
    // {
    //     double x = unif_1(re); // used of % here is just fine
    //     double y = unif_2(re);
    //     node node_t(x, y, 5, 1);
    //     vertices.push_back(node_t);
    // }

    // assert(this->vertices.size() == this->n_nodes + 1);
}

double graph::dist(node u, node v)
{
    return sqrt(pow(u.x - v.x, 2) + pow(u.y - v.y, 2));
}

int graph::add_node(double _x, double _y, int _node_weight, int _priority)
{
    node new_node(_x, _y, _node_weight, _priority);

    if (not check_double_node(new_node))
    {
        // cout << "[ERROR:graph]:: double node! Skipped" << endl;
        return -1;
    }
    if (new_node.x > this->area_x or new_node.y > this->area_y)
    {
        // cout << "[ERROR:graph]:: node outside area" << endl;
        return -2;
    }

    this->vertices.push_back(new_node);
    this->n_nodes++;

    return 1;
}

bool graph::check_double_node(node new_node)
{
    for (size_t i = 0; i < this->vertices.size(); i++)
    {
        node temp = this->vertices[i];
        if (temp.x == new_node.x and temp.y == new_node.y)
        {
            return false;
        }
    }
    return true;
}

void graph::print_graph()
{
    cout << "***** GRAPH *****" << endl;
    for (size_t i = 0; i < this->vertices.size(); i++)
    {
        cout << i << ": (" << this->vertices[i].y << ", " << this->vertices[i].y << ") p:" << this->vertices[i].priority << " w:" << this->vertices[i].node_weight << endl;
    }
    cout << "*****************" << endl;
}