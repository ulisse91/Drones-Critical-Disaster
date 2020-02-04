#include <iostream>

#include <boost/program_options.hpp>

#include "main.h"
#include "graph.h"

using namespace std;
namespace po = boost::program_options;

int main(int argc, char **argv)
{    
    po::options_description desc{"Options"};
    desc.add_options()
        ("help,h", "Help screen")
        ("nodes,n", po::value<int>(), "Number of nodes");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);    

    if (vm.count("help"))
    {
        cout << desc << '\n';
        return 1;
    }
    if (vm.count("nodes")) {
        n_nodes = vm["nodes"].as<int>();
        cout << "Number of nodes: " << vm["nodes"].as<int>() << endl;
    } else {
        cout << "Number of nodes was not set.\n";
        cout << "Set Default value: " << n_nodes << "\n";
    }

    graph G = graph(1,2,n_nodes);

    for (size_t i = 0; i < G.n_nodes; i++)
    {
        cout << "(" << G.vertices[i].x << "," << G.vertices[i].y << "), ";
    }
    

    return 0;
}