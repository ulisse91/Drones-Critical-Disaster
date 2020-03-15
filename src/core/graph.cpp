#include "graph.h"

graph::graph(int _area_x, int _area_y)
{
    this->area_x = _area_x;
    this->area_y = _area_y;

    // DEPOT: must have priority and weight == 0 (we use this in the code)
    this->vertices[0] = node(0, 0, 0, 0, 0); // v_0 = depot
    this->n_nodes = 1;
}

graph::graph() {}
graph::~graph() {}

void graph::create_random_graph(int number_of_nodes, double max_weight, int max_priority)
{
    create_random_graph(number_of_nodes, max_weight, max_priority, std::random_device{}());
}

void graph::create_random_graph(int number_of_nodes, double max_weight, int max_priority, long seed)
{
    erase_graph();

    if (seed == -1)
        seed = std::random_device{}();

    std::uniform_real_distribution<double> unif_1(0, this->area_x);
    std::uniform_real_distribution<double> unif_2(0, this->area_y);
    std::uniform_int_distribution<int> unif_3(1, max_weight + 1);
    std::uniform_real_distribution<double> unif_4(1, max_priority + 1);

    //Mersenne Twister: Good quality random number generator
    std::mt19937 re(seed);

    for (size_t i = 0; i < number_of_nodes; i++)
    {
        double _x = unif_1(re);
        double _y = unif_2(re);
        double _w = unif_3(re);
        int _p = unif_4(re);

        this->add_node(_x, _y, _w, _p);
    }
    assert(this->vertices.size() == this->n_nodes);
}

int graph::add_node(int id, double _x, double _y, double _node_weight, int _priority)
{
    if (not check_double_node(_x, _y))
    {
        // std::cerr << "[ERROR:graph]:: double node! Skipped" << std::endl;
        return -1;
    }
    if (_x > this->area_x or _y > this->area_y)
    {
        // std::cerr << "[ERROR:graph]:: node outside area" << std::endl;
        return -2;
    }

    this->vertices[id] = node(id, _x, _y, _node_weight, _priority);
    this->n_nodes++;

    return 1;
}

int graph::add_node(double _x, double _y, double _node_weight, int _priority)
{
    if (not check_double_node(_x, _y))
    {
        // std::cerr << "[ERROR:graph]:: double node! Skipped" << std::endl;
        return -1;
    }
    if (_x > this->area_x or _y > this->area_y)
    {
        // std::cerr << "[ERROR:graph]:: node outside area" << std::endl;
        return -2;
    }

    this->vertices[this->n_nodes] = node(this->n_nodes, _x, _y, _node_weight, _priority);
    this->n_nodes++;

    return 1;
}

bool graph::check_double_node(double _x, double _y)
{
    for (auto const &pair : this->vertices)
    {
        if (_x == pair.second.x and _y == pair.second.y)
        {
            return false;
        }
    }
    return true;
}

/////////////////////////////////////////////////
//////////////////// I/O ////////////////////////
/////////////////////////////////////////////////

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

void graph::erase_graph()
{
    this->n_nodes = 1;
    this->vertices = std::map<int, node>();
    this->vertices[0] = node(0, 0, 0, 0, 0); // v_0 = depot
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
    for (auto const &pair : this->vertices)
    {
        assert(pair.first == pair.second.id);
        nodes_id.push_back(pair.first);
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
    for (auto const &pair : this->vertices)
    {
        assert(pair.first == pair.second.id);
        nodes_id.insert(pair.first);
    }
    return nodes_id;
}

/////////////////////////////////////////////////
//////////////// DISTANCES //////////////////////
/////////////////////////////////////////////////

double graph::distw(int u, int v)
{
    return dist(u, v) + this->vertices[u].node_weight / 2 + this->vertices[v].node_weight / 2;
}

double graph::dist(int u, int v)
{
    return sqrt(pow(this->vertices[u].x - this->vertices[v].x, 2) + pow(this->vertices[u].y - this->vertices[v].y, 2));
}

double graph::dist(int u, int v, int with_weight)
{
    return with_weight ? distw(u, v) : dist(u, v);
}