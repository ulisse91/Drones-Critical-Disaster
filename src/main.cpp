#include <iostream>

#include <boost/program_options.hpp>

#include "core/main.h"
#include "core/graph.h"
#include "core/simulator.h"

namespace po = boost::program_options;

int main(int argc, char **argv)
{
    po::options_description desc{"Options"};
    desc.add_options()("help,h", "Help screen")("nodes,n", po::value<int>(), "Number of nodes (default = 10)")("drones,d", po::value<int>(), "Number of drones (default = 1)")("budget,b", po::value<double>(), "Budget (default = 1)")("batteries,p", po::value<int>(), "Number of batteries (default = number of drones)")("file,f", po::value<std::string>(), "Graph file");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << '\n';
        return 1;
    }
    if (vm.count("nodes"))
    {
        n_nodes = vm["nodes"].as<int>();
        std::cout << "Number of nodes: " << vm["nodes"].as<int>() << std::endl;
    }
    else
    {
        std::cout << "Number of nodes was not set. ";
        std::cout << "Set Default value: " << n_nodes << "\n";
    }
    if (vm.count("drones"))
    {
        n_drones = vm["drones"].as<int>();
        std::cout << "Number of drones: " << vm["drones"].as<int>() << std::endl;
    }
    else
    {
        std::cout << "Number of drones was not set. ";
        std::cout << "Set Default value: " << n_drones << "\n";
    }
    if (vm.count("batteries"))
    {
        n_batteries = vm["batteries"].as<int>();
        std::cout << "Number of batteries: " << vm["batteries"].as<int>() << std::endl;
    }
    else
    {
        std::cout << "Number of batteries was not set. ";
        std::cout << "Set Default value: " << n_batteries << "\n";
    }
    if (vm.count("budget"))
    {
        budget = vm["budget"].as<double>();
        std::cout << "Budget: " << vm["budget"].as<double>() << std::endl;
    }
    else
    {
        std::cout << "Budget was not set. ";
        std::cout << "Set Default value: " << budget << "\n";
    }
    if (vm.count("file"))
    {
        graph_file = vm["file"].as<std::string>();
        std::cout << "file: " << vm["file"].as<std::string>() << std::endl;
    }
    else
    {
        std::cout << "file was not set. ";
    }

    graph G = graph(2, 1);
    G.read_graph_from_file(graph_file);
    G.print_graph();

    // fun: read solution from file (?)
    // alg: KIM
    // alg: prim-based
    // alg: TOP-based
    // alg: greedy algorithm (for benchmark)
    // create random graph -> populate_graph() diventa random graph

    return 0;
}