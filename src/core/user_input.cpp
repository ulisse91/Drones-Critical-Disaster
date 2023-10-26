#include "user_input.h"

std::string userinput::UNIFORM = "uniform";
std::string userinput::POISSON = "poisson";

input userinput::read_user_input(int argc, char **argv)
{
    std::map<std::string, int> experiment_map =
        {{"generate-graph", 1},
         {"test-batteries", 2},
         {"full-simulation", 0},
         {"top-comparison", 3},
         {"print-cycles", 4},
         {"generate-graph-multi-depot", 5},
         {"multi-depot", 6},
         {"generate-drones-multi-depot", 7},
         {"divide-graph-four-quadrants", 8},
         {"testing_one_depot", 9}};

    time_t my_time = time(NULL);
    std::cout << std::endl
              << ctime(&my_time) << std::endl;

    input n_input = input();

    po::options_description desc{"Options"};
    desc.add_options()("help,h", "Help screen")("nodes,n", po::value<int>(), "[int] Number of nodes (default = 10)")("drones,q", po::value<int>(), "[int] Number of drones (default = 1)")("budget,b", po::value<double>(), "[double] Budget (default = 1)")("progsigma,p", po::value<double>(), "[double] Probability sigma prime (default = 0). The value must be between 0 and 1")("depots,d", po::value<int>(), "[int] number of depots (default = 1)")("seed,s", po::value<int>(), "[int] seed for random graph generator")("batteries,m", po::value<int>(), "[int] Number of batteries (default = number of drones). The value must be greater than number of drones")("graphfile,f", po::value<std::string>(), "[path] Graph file")("dronesfile,u", po::value<std::string>(), "[path] Drones file")("distrib,z", po::value<std::string>(), "[string] Distribution (default = \"uniform\"). Possible values: uniform, poisson")("simulation", po::value<std::string>(), "[string] Which simulation/experiment to launch (default = \"full-simulation\"). Possible values: generate-graph (1), test-batteries (2), full-simulation (0), top-comparison (3), print-cycles (4)")("e", po::value<int>(), "[int] Which simulation/experiment to launch (default = 0). Possible values: see \"simulation\"");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << '\n';
        exit(0);
    }
    else
    {
        if (vm.count("simulation"))
        {
            if (experiment_map.find(vm["simulation"].as<std::string>()) == experiment_map.end())
            {
                std::cout << "[ERROR] Simulation: \"" << vm["simulation"].as<std::string>() << "\" does not exist" << std::endl;
                exit(EXIT_FAILURE);
            }
            else
            {
                n_input.experiment = experiment_map[vm["simulation"].as<std::string>()];
            }
            std::cout << "Experiment: " << vm["simulation"].as<std::string>() << std::endl;
        }
        else
        {
            if (vm.count("e"))
            {
                n_input.experiment = vm["e"].as<int>();
                std::cout << "Experiment: " << vm["e"].as<int>() << std::endl;
            }
            else
            {
                std::cout << "Experiment was not set. ";
                std::cout << "Set Default value: " << n_input.experiment << std::endl;
            }
        }

        if (vm.count("graphfile"))
        {
            n_input.graph_file = vm["graphfile"].as<std::string>();
            std::cout << "Graph-file: " << vm["graphfile"].as<std::string>() << std::endl;
        }
        else if (vm.count("generate-drones-multi-depot"))
        {
            std::cout << "generate-drones-multi-depot" << std::endl;
        }
        else
        {
            std::cout << "Graph-File was not set." << std::endl;
            if (n_input.experiment == 6)
            {
                std::cout << "Simulation Multi-depot requires a graph file!\nEnding Simulation...\n"
                          << std::endl;
                assert(false);
            }
            else
            {
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
                if (vm.count("distrib"))
                {
                    n_input.distrib = vm["distrib"].as<std::string>();
                    std::cout << "Distribution: " << vm["distrib"].as<std::string>() << std::endl;
                }
                else
                {
                    std::cout << "Random distribution was not set. ";
                    std::cout << "Set Default value: " << n_input.distrib << std::endl;
                }
            }
        }

        if (n_input.experiment == 6)
        {
            if (vm.count("dronesfile"))
            {
                n_input.drones_file = vm["dronesfile"].as<std::string>();
                std::cout << "Drones-file: " << vm["dronesfile"].as<std::string>() << std::endl;
            }
            else
            {
                std::cout << "Simulation Multi-depot requires a drones file!\nDrones-File was not set.\nEnding Simulation...\n"
                          << std::endl;
                assert(false);
            }
        }
        else
        {

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
            if (vm.count("depots"))
            {
                n_input.n_depots = vm["depots"].as<int>();
                std::cout << "Number of depots: " << vm["depots"].as<int>() << std::endl;
                if (not vm.count("depots"))
                {
                    std::cout << "Number of depots was not set. ";
                    std::cout << "Set Default value: " << n_input.n_depots << std::endl;
                }
            }
            else
            {
                std::cout << "Number of depots was not set. ";
                std::cout << "Set Default value: " << n_input.n_depots << std::endl;
            }
            if (vm.count("batteries"))
            {
                n_input.n_batteries = vm["batteries"].as<int>();
                if (n_input.n_batteries < n_input.n_drones)
                {
                    std::cout << "[ERROR] Number of batteries less than number of drones" << std::endl;
                    exit(EXIT_FAILURE);
                }
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
            if (vm.count("progsigma"))
            {
                n_input.prob_sigma_prime = vm["progsigma"].as<double>();

                if (1 < n_input.prob_sigma_prime or n_input.prob_sigma_prime < 0)
                {
                    std::cout << "[ERROR] The value of \"Probability sigma prime\" must be between 0 and 1" << std::endl;
                    exit(EXIT_FAILURE);
                }
                std::cout << "progsigma: " << vm["progsigma"].as<double>() << std::endl;
            }
            else
            {
                std::cout << "Probability sigma prime: 0 (default value)" << std::endl;
            }
        }
    }
    std::cout << std::endl;

    return n_input;
}

void userinput::print_input(input n_input)
{
    std::cout << "n_nodes: " << n_input.n_nodes << "\n"
              << "n_drones:" << n_input.n_drones << "\n"
              << "n_batteries: " << n_input.n_batteries << "\n"
              << "n_depots: " << n_input.n_depots << "\n"
              << "budget: " << n_input.budget << "\n"
              << "seed: " << n_input.seed << "\n"
              << "prob_sigma_prime: " << n_input.prob_sigma_prime << "\n"
              << "graph_file: " << n_input.graph_file << "\n"
              << "distrib: " << n_input.distrib << "\n"
              << "experiment: " << n_input.experiment << "\n";
}