#include "graph.h"

graph::graph(int _area_x, int _area_y)
{
    this->area_x = _area_x;
    this->area_y = _area_y;

    // DEPOT: must have priority and weight == 0 (we use this in the code)
    node depot(0, 0, 0, 0, 0); // v_0 = depot
    this->vertices.push_back(depot);

    this->n_nodes = 1;
}

graph::graph() {}

graph::~graph() {}

void graph::create_random_graph()
{
    std::cout << "TO_DO\n";
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

double graph::dist(int u, int v)
{
    return sqrt(pow(this->vertices[u].x - this->vertices[v].x, 2) + pow(this->vertices[u].y - this->vertices[v].y, 2));
}

int graph::add_node(int id, double _x, double _y, double _node_weight, int _priority)
{
    node new_node(id, _x, _y, _node_weight, _priority);

    if (not check_double_node(new_node))
    {
        // std::cout << "[ERROR:graph]:: double node! Skipped" << std::endl;
        return -1;
    }
    if (new_node.x > this->area_x or new_node.y > this->area_y)
    {
        // std::cout << "[ERROR:graph]:: node outside area" << std::endl;
        return -2;
    }

    this->vertices.push_back(new_node);
    this->n_nodes++;

    return 1;
}

int graph::add_node(double _x, double _y, double _node_weight, int _priority)
{
    node new_node(this->n_nodes, _x, _y, _node_weight, _priority);

    if (not check_double_node(new_node))
    {
        // std::cout << "[ERROR:graph]:: double node! Skipped" << std::endl;
        return -1;
    }
    if (new_node.x > this->area_x or new_node.y > this->area_y)
    {
        // std::cout << "[ERROR:graph]:: node outside area" << std::endl;
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
    std::cout << "***** GRAPH *****" << std::endl;
    for (size_t i = 0; i < this->vertices.size(); i++)
    {
        std::cout << this->vertices[i].id << ": (" << this->vertices[i].x << ", " << this->vertices[i].y << ") p:" << this->vertices[i].priority << " w:" << this->vertices[i].node_weight << std::endl;
    }
    std::cout << "*****************" << std::endl;
}

int graph::read_graph_from_file(std::string file)
{
    std::fstream fin;
    fin.open(file, std::ios::in);

    if (fin.is_open())
    {
        std::string _x, _y, _priority, _node_weight;
        while (fin.good())
        {
            getline(fin, _x, ',');
            getline(fin, _y, ',');
            getline(fin, _priority, ',');
            getline(fin, _node_weight);
            add_node(stod(_x), stod(_y), stod(_node_weight), stoi(_priority));
        }
        fin.close();
        return 1;
    }
    return -1; // Unable to open file
}

/////////////////////////////////////////////////
/////////////// GETTER & SETTER /////////////////
/////////////////////////////////////////////////

int graph::get_area_x()
{
    return this->area_x;
}

int graph::get_area_y()
{
    return this->area_y;
}

int graph::get_n_nodes()
{
    return this->n_nodes;
}

std::vector<int> graph::get_vertices()
{
    std::vector<int> nodes_id;
    for (size_t i = 0; i < this->n_nodes; i++)
    {
        nodes_id.push_back(this->vertices[i].id);
    }
    return nodes_id;
}

int graph::get_priority_node(int id)
{
    return this->vertices[id].priority;
}

double graph::get_weight_node(int id)
{
    return this->vertices[id].node_weight;
}

double graph::get_coord_x(int id)
{
    return this->vertices[id].x;
}

double graph::get_coord_y(int id)
{
    return this->vertices[id].y;
}

std::unordered_set<int> graph::get_vertices_set()
{
    std::unordered_set<int> nodes_id;
    for (size_t i = 0; i < this->n_nodes; i++)
    {
        nodes_id.insert(this->vertices[i].id);
    }
    return nodes_id;
}