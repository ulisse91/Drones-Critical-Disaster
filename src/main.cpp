#include <assert.h>
#include <fstream>
#include <chrono>

#include <boost/algorithm/string.hpp>

#include "core/user_input.h"
#include "simulator/simulator.h"

int main(int argc, char **argv)
{
    input n_input = userinput::read_user_input(argc, argv);
    std::ofstream outfile;

    graph G = graph(1, 1);
    if (n_input.distrib == userinput::UNIFORM)
    {
        G.create_random_graph(n_input.n_nodes, 2, 3, n_input.seed);
        // G.create_random_graph(n_input.n_nodes, 0, 0, n_input.seed);
    }
    if (n_input.distrib == userinput::POISSON)
    {
        G.create_random_graph_poisson(n_input.n_nodes, 2, 3, n_input.seed);
    }
    print::print_graph(G);
    simulator sim = simulator(G, n_input.n_drones, n_input.n_drones /* batteries */, n_input.budget, n_input.prob_sigma_prime, n_input.seed);

    assert(sim.check_feasibility());

    std::vector<std::string> algs = {"PRIM", "TOP", "GMAX", "GMIN", "TOP-PRIM", "TOP-GREEDY", "GMAX-ROUND", "TOP 2-apx", "TOP-GMAX-ROUND"};

    auto start_t = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol = sim.meta_algorithm(6);
    auto stop_t = std::chrono::high_resolution_clock::now();
    std::cout << std::endl
              << algs[6] << " ALGORITHM" << std::endl;
    print::print_e_solution(G, sol, sim.sigma_prime_probs);
    std::cout << "fun_cycle: " << sim.evaluate_solution(0, sol) << " func_weighted_latency: " << sim.evaluate_solution(1, sol) << " func_completion_time: " << sim.evaluate_solution(2, sol) << " | time(mus): " << std::chrono::duration_cast<std::chrono::microseconds>(stop_t - start_t).count() << std::endl;
    std::vector<double> ct_graphs_p = sim.completion_time_priorities(sol);
    print::print_vector(ct_graphs_p);
    std::vector<double> test = utilities::stat_sol(G, sol, sim.sigma_prime_probs, n_input.budget);
    std::cout << "number of cycles: " << test[0] << " avg time in cycles: " << test[1] << " min budget spent in cycles: " << test[2] << std::endl;
    assert(sim.check_solution_feasible(sol) == 1);
    std::cout << n_input.n_nodes                                                                        /* number of nodes */
              << "," << n_input.budget                                                                  /* budget */
              << "," << n_input.n_drones                                                                /* number of drones */
              << "," << n_input.prob_sigma_prime                                                        /* probability per sigma prime */
              << "," << n_input.seed                                                                    /* seed */
              << "," << sim.evaluate_solution(0, sol)                                                   /* wL^I */
              << "," << sim.evaluate_solution(1, sol)                                                   /* wL^II */
              << "," << sim.evaluate_solution(2, sol)                                                   /* completion time */
              << "," << ct_graphs_p[0]                                                                  /* completion time graph max */
              << "," << ct_graphs_p[1]                                                                  /* completion time graph med */
              << "," << ct_graphs_p[2]                                                                  /* completion time graph min */
              << "," << std::chrono::duration_cast<std::chrono::microseconds>(stop_t - start_t).count() /* microsends computation algorithm */
              << "," << test[0]                                                                         /* number of cycles in solution */
              << "," << test[1]                                                                         /* average time cycles (except last cycle) */
              << "," << test[2]                                                                         /* min time cycles (except last cycle) */
              << "\n";

    for (int i = 0; i < 2*n_input.n_drones +1; i++)
    {
        std::cout << i << " " << sim.obj_ct_batteries(sol, i) << std::endl;
    }
    
    return 0;

    
    for (int i = 0; i < 9; i++)
    {
        if (i == 2 or i == 3 or i == 4 or i == 5 or i == 7)
            continue; // skip some of the algs
        auto start_t = std::chrono::high_resolution_clock::now();
        std::vector<std::vector<std::vector<std::pair<int, double>>>> sol = sim.meta_algorithm(i);
        auto stop_t = std::chrono::high_resolution_clock::now();
        std::cout << std::endl
                  << algs[i] << " ALGORITHM" << std::endl;
        print::print_e_solution(G, sol, sim.sigma_prime_probs);
        std::cout << "fun_cycle: " << sim.evaluate_solution(0, sol) << " func_weighted_latency: " << sim.evaluate_solution(1, sol) << " func_completion_time: " << sim.evaluate_solution(2, sol) << " | time(mus): " << std::chrono::duration_cast<std::chrono::microseconds>(stop_t - start_t).count() << std::endl;
        std::vector<double> ct_graphs_p = sim.completion_time_priorities(sol);
        print::print_vector(ct_graphs_p);
        std::vector<double> test = utilities::stat_sol(G, sol, sim.sigma_prime_probs, n_input.budget);
        std::cout << "number of cycles: " << test[0] << " avg time in cycles: " << test[1] << " min budget spent in cycles: " << test[2] << std::endl;
        assert(sim.check_solution_feasible(sol) == 1);
        outfile.open("data/output/" + boost::algorithm::to_lower_copy(algs[i]) + "_" + n_input.distrib + ".csv", std::ios_base::app);
        outfile << n_input.n_nodes                                                                        /* number of nodes */
                << "," << n_input.budget                                                                  /* budget */
                << "," << n_input.n_drones                                                                /* number of drones */
                << "," << n_input.prob_sigma_prime                                                        /* probability per sigma prime */
                << "," << n_input.seed                                                                    /* seed */
                << "," << sim.evaluate_solution(0, sol)                                                   /* wL^I */
                << "," << sim.evaluate_solution(1, sol)                                                   /* wL^II */
                << "," << sim.evaluate_solution(2, sol)                                                   /* completion time */
                << "," << ct_graphs_p[0]                                                                  /* completion time graph max */
                << "," << ct_graphs_p[1]                                                                  /* completion time graph med */
                << "," << ct_graphs_p[2]                                                                  /* completion time graph min */
                << "," << std::chrono::duration_cast<std::chrono::microseconds>(stop_t - start_t).count() /* microsends computation algorithm */
                << "," << test[0]                                                                         /* number of cycles in solution */
                << "," << test[1]                                                                         /* average time cycles (except last cycle) */
                << "," << test[2]                                                                         /* min time cycles (except last cycle) */
                << "\n";
        outfile.close();
    }

    std::cout << "================================================\n";
    std::cout << "================================================\n";
    std::cout << "================================================\n\n";

    return 0;
}