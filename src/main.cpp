#include <assert.h>
#include <fstream>
#include <chrono>

#include "core/user_input.h"
#include "simulator/simulator.h"

int main(int argc, char **argv)
{
    input n_input = userinput::read_user_input(argc, argv);
    std::ofstream outfile;

    graph G = graph(2, 2);
    if (n_input.graph_file != "")
    {
        G.read_graph_from_file(n_input.graph_file);
    }
    else
    {
        G.create_random_graph(n_input.n_nodes, 3, 3, n_input.seed);
    }

    print::print_graph(G);

    simulator sim = simulator(G, n_input.n_drones, n_input.n_drones /* batteries */, n_input.budget, n_input.prob_sigma_prime, n_input.seed);

    assert(sim.check_feasibility());

    std::vector<std::string> algs = {"PRIM BASED", "TOP BASED", "GMAX", "GMIN", "T+P"};
    std::vector<std::string> files = {"prim.csv", "top.csv", "gmax.csv", "gmin.csv", "topprim.csv"};

    for (int i = 0; i < 5; i++)
    {
        auto start_t = std::chrono::high_resolution_clock::now();
        std::vector<std::vector<std::vector<std::pair<int, double>>>> sol = sim.meta_algorithm(i);
        auto stop_t = std::chrono::high_resolution_clock::now();
        std::cout << algs[i] << " ALGORITHM" << std::endl;
        print::print_e_solution(G, sol, sim.sigma_prime_probs);
        std::cout << "fun_cycle: " << sim.evaluate_solution(0, sol) << " func_weighted_latency: " << sim.evaluate_solution(1, sol) << " func_completion_time: " << sim.evaluate_solution(2, sol) << " | time(ms): " << std::chrono::duration_cast<std::chrono::milliseconds>(stop_t - start_t).count() << std::endl;
        std::vector<double> ct_graphs_p = sim.completion_time_priorities(sol);
        print::print_vector(ct_graphs_p);
        std::cout << std::endl;
        assert(sim.check_solution_feasible(sol) == 1);
        outfile.open("data/output/"+files[i], std::ios_base::app);
        outfile << n_input.n_nodes << "," << n_input.budget << "," << n_input.n_drones << "," << n_input.prob_sigma_prime << "," << n_input.seed << "," << sim.evaluate_solution(0, sol) << "," << sim.evaluate_solution(1, sol) << "," << sim.evaluate_solution(2, sol) << "," << ct_graphs_p[0]<< "," << ct_graphs_p[1] << "," << ct_graphs_p[2]<< "\n";
        outfile.close();
    }

    std::cout << "================================================\n";
    std::cout << "================================================\n";
    std::cout << "================================================\n\n";

    return 0;
}