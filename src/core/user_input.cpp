#include "user_input.h"

input userinput::read_user_input(int argc, char **argv)
{
    time_t my_time = time(NULL);
    std::cout << std::endl
              << ctime(&my_time) << std::endl;

    input n_input = input();

    po::options_description desc{"Options"};
    desc.add_options()("help,h", "Help screen")("nodes,n", po::value<int>(), "Number of nodes (default = 10)")("drones,d", po::value<int>(), "Number of drones (default = 1)")("budget,b", po::value<double>(), "Budget (default = 1)")("seed,s", po::value<int>(), "seed for random graph generator")("batteries,p", po::value<int>(), "Number of batteries (default = number of drones)")("file,f", po::value<std::string>(), "Graph file");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << '\n';
    }
    if (vm.count("file"))
    {
        n_input.graph_file = vm["file"].as<std::string>();
        std::cout << "Graph-file: " << vm["file"].as<std::string>() << std::endl;
    }
    else
    {
        std::cout << "file was not set." << std::endl;
        if (vm.count("nodes"))
        {
            n_input.n_nodes = vm["nodes"].as<int>();
            std::cout << "Number of nodes: " << vm["nodes"].as<int>() << std::endl;
        }
        else
        {
            std::cout << "Number of nodes was not set. ";
            std::cout << "Set Default value: " << n_input.n_nodes << std::endl;
        }
    }
    if (vm.count("drones"))
    {
        n_input.n_drones = vm["drones"].as<int>();
        std::cout << "Number of drones: " << vm["drones"].as<int>() << std::endl;
        if (not vm.count("batteries"))
        {
            std::cout << "Number of batteries set equal to number of drones" << std::endl;
            n_input.n_batteries = n_input.n_drones;
        }
    }
    else
    {
        std::cout << "Number of drones was not set. ";
        std::cout << "Set Default value: " << n_input.n_drones << std::endl;
    }
    if (vm.count("batteries"))
    {
        n_input.n_batteries = vm["batteries"].as<int>();
        std::cout << "Number of batteries: " << vm["batteries"].as<int>() << std::endl;
    }
    if (vm.count("budget"))
    {
        n_input.budget = vm["budget"].as<double>();
        std::cout << "Budget: " << vm["budget"].as<double>() << std::endl;
    }
    else
    {
        std::cout << "Budget was not set. ";
        std::cout << "Set Default value: " << n_input.budget << std::endl;
    }
    if (vm.count("seed"))
    {
        n_input.seed = vm["seed"].as<int>();
        std::cout << "Seed: " << vm["seed"].as<int>() << std::endl;
    }
    else
    {
        std::cout << "Seed: 0 (default value)" << std::endl;
    }
    std::cout << std::endl;

    return n_input;
}