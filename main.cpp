#include <iostream>

#include <boost/program_options.hpp>

#include "main.h"
#include "graph.h"
#include "simulator.h"

using namespace std;
namespace po = boost::program_options;

int main(int argc, char **argv)
{
    po::options_description desc{"Options"};
    desc.add_options()("help,h", "Help screen")("nodes,n", po::value<int>(), "Number of nodes (default = 10)")("drones,d", po::value<int>(), "Number of drones (default = 1)")("budget,b", po::value<int>(), "Budget (default = 1)")("batteries,p", po::value<int>(), "Number of batteries (default = number of drones)");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        cout << desc << '\n';
        return 1;
    }
    if (vm.count("nodes"))
    {
        n_nodes = vm["nodes"].as<int>();
        cout << "Number of nodes: " << vm["nodes"].as<int>() << endl;
    }
    else
    {
        cout << "Number of nodes was not set. ";
        cout << "Set Default value: " << n_nodes << "\n";
    }
    if (vm.count("drones"))
    {
        n_drones = vm["drones"].as<int>();
        cout << "Number of drones: " << vm["drones"].as<int>() << endl;
    }
    else
    {
        cout << "Number of drones was not set. ";
        cout << "Set Default value: " << n_drones << "\n";
    }
    if (vm.count("batteries"))
    {
        n_batteries = vm["batteries"].as<int>();
        cout << "Number of batteries: " << vm["batteries"].as<int>() << endl;
    }
    else
    {
        cout << "Number of batteries was not set. ";
        cout << "Set Default value: " << n_batteries << "\n";
    }
    if (vm.count("budget"))
    {
        budget = vm["budget"].as<int>();
        cout << "Budget: " << vm["budget"].as<int>() << endl;
    }
    else
    {
        cout << "Budget was not set. ";
        cout << "Set Default value: " << budget << "\n";
    }

    //// TEST
    graph G = graph(1, 2, n_nodes);
    simulator sim = simulator(G, n_drones, n_batteries, budget);

    G.print_graph();

    vector<vector<vector<pair<int, double>>>> sol_2 = {{{make_pair(0, 1), make_pair(1, 0.5), make_pair(2, 1), make_pair(0, 1)}}, {{make_pair(0, 1) , make_pair(1, 0.5),make_pair(4, 1), make_pair(5, 1), make_pair(0, 1)}}};

    cout << sim.evaluate_solution(0, sol_2) << endl;
    //// END TEST

    // input: file_graph
    // fun: read graph from file
    // fun: read solution from file (?)
    // alg: KIM
    // enanche:: check_solution_feasible deve contenere: se sono stati fatti tutti i nodi e "fino in fondo"
    // create random graph -> populate_graph() diventa random graph

    return 0;
}