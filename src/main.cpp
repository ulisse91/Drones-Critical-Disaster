#include <assert.h>
#include <fstream>
#include <chrono>

#include <boost/algorithm/string.hpp>

#include "core/user_input.h"
#include "simulator/simulator.h"

/*
    batteries_greedy:
    simulate the problem using algorithm Greedy round with a number of batteries equal to q+b, with 0<= b <= 2q
    The function produces an output of the form:
    <number of batteries> <value of function ct> 
*/
void batteries_greedy(input n_input)
{
    graph G = graph(1, 1);
    if (n_input.distrib == userinput::UNIFORM)
    {
        G.create_random_graph(n_input.n_nodes, 2, 3, n_input.seed);
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
    std::cout << "wL^I: " << sim.evaluate_solution(0, sol) << " wL^II: " << sim.evaluate_solution(1, sol) << " ct: " << sim.evaluate_solution(2, sol) << " | time(mus): " << std::chrono::duration_cast<std::chrono::microseconds>(stop_t - start_t).count() << std::endl;
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

    double recharge_time = 2.4;
    int limit = std::min(std::ceil(recharge_time) * n_input.n_drones, test[0] - n_input.n_drones);

    for (int i = 0; i < limit + 1; i++)
    {
        std::cout << i << " " << sim.obj_ct_batteries(recharge_time, sol, i) << std::endl;
    }
}

/*
    full_simulation simulates only:
    PRIM - TOP (heuristic) - Greedy round - TOP+Greedy round
*/
void full_simulation(input n_input)
{
    std::ofstream outfile;
    graph G = graph(1, 1);
    if (n_input.distrib == userinput::UNIFORM)
    {
        G.create_random_graph(n_input.n_nodes, 2, 3, n_input.seed);
    }
    if (n_input.distrib == userinput::POISSON)
    {
        G.create_random_graph_poisson(n_input.n_nodes, 2, 3, n_input.seed);
    }
    print::print_graph(G);
    simulator sim = simulator(G, n_input.n_drones, n_input.n_drones /* batteries */, n_input.budget, n_input.prob_sigma_prime, n_input.seed);

    assert(sim.check_feasibility());

    std::vector<std::string> algs = {"PRIM", "TOP", "GMAX", "GMIN", "TOP-PRIM", "TOP-GREEDY", "GMAX-ROUND", "TOP 2-apx", "TOP-GMAX-ROUND"};

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
        std::cout << "wL^I: " << sim.evaluate_solution(0, sol) << " wL^II: " << sim.evaluate_solution(1, sol) << " ct: " << sim.evaluate_solution(2, sol) << " | time(mus): " << std::chrono::duration_cast<std::chrono::microseconds>(stop_t - start_t).count() << std::endl;
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
}

void top_comparison(input n_input)
{
    std::ofstream outfile;

    graph G = graph(1, 1);
    if (n_input.distrib == userinput::UNIFORM)
    {
        G.create_random_graph(n_input.n_nodes, 2, 3, n_input.seed);
    }
    if (n_input.distrib == userinput::POISSON)
    {
        G.create_random_graph_poisson(n_input.n_nodes, 2, 3, n_input.seed);
    }
    print::print_graph(G);

    simulator sim = simulator(G, n_input.n_drones, n_input.n_drones /* batteries */, n_input.budget, n_input.prob_sigma_prime, n_input.seed);

    assert(sim.check_feasibility());

    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol_apx = sim.meta_algorithm(7);
    std::cout << "---------------------\n";
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol_heur = sim.meta_algorithm(1);

    print::print_e_solution(G, sol_apx, sim.sigma_prime_probs);
    print::print_e_solution(G, sol_heur, sim.sigma_prime_probs);

    assert(sim.check_solution_feasible(sol_apx) == 1);
    assert(sim.check_solution_feasible(sol_heur) == 1);

    outfile.open("data/output/confronto_TOP_" + n_input.distrib + ".csv", std::ios_base::app);
    outfile << n_input.n_nodes                           /* number of nodes */
            << "," << n_input.budget                     /* budget */
            << "," << n_input.n_drones                   /* number of drones */
            << "," << n_input.prob_sigma_prime           /* probability per sigma prime */
            << "," << n_input.seed                       /* seed */
            << "," << sim.evaluate_solution(0, sol_apx)  /* wL^I */
            << "," << sim.evaluate_solution(1, sol_apx)  /* wL^II */
            << "," << sim.evaluate_solution(2, sol_apx)  /* completion time */
            << "," << sim.evaluate_solution(0, sol_heur) /* wL^I */
            << "," << sim.evaluate_solution(1, sol_heur) /* wL^II */
            << "," << sim.evaluate_solution(2, sol_heur) /* completion time */
            << "\n";
    outfile.close();
}

/*
    Ogni file ha contenuto nel nome le seguenti informazioni (in ordine):
    - n = numero di nodi
    - B = budget
    - p = probabilita' sigma'
    - s = seed per la generazione dei dati
    - distribuzione utilizzata per la creazione dei punti nel piano [uniform/poisson]

    Esempio: 
    file graph_n20_B50_p0.750000_s100000_uniform.csv
    si hanno 20 nodi
    budget 50
    probabilita' sigma' 0.75
    seed usato per la generazione 100000
    distribuzione uniforme per la generazione dei punti nel piano
*/
void print_graph_to_file(input n_input)
{
    graph G = graph(1, 1);
    if (n_input.distrib == userinput::UNIFORM)
    {
        G.create_random_graph(n_input.n_nodes, 2, 3, n_input.seed);
    }
    if (n_input.distrib == userinput::POISSON)
    {
        G.create_random_graph_poisson(n_input.n_nodes, 2, 3, n_input.seed);
    }
    // print::print_graph(G);
    simulator sim = simulator(G, n_input.n_drones, n_input.n_drones /* batteries */, n_input.budget, n_input.prob_sigma_prime, n_input.seed);

    assert(sim.check_feasibility());

    std::string nome_file = "data/graph/generated/graph_n" + std::to_string(n_input.n_nodes) + "_B" + std::to_string((int)n_input.budget) + "_p" + std::to_string(n_input.prob_sigma_prime) + "_s" + std::to_string(n_input.seed) + "_" + n_input.distrib + ".csv";

    print::print_graph_to_file(G, sim.sigma_prime_probs, nome_file);

    return;
}
/*
    print_cycles prints the ordinate coordinates of each cycle of the solution sequence
    for algorithm:
    PRIM - Greedy Round
*/
void print_cycles(input n_input)
{
    std::ofstream outfile;
    graph G = graph(1, 1);
    if (n_input.distrib == userinput::UNIFORM)
    {
        G.create_random_graph(n_input.n_nodes, 2, 3, n_input.seed);
    }
    if (n_input.distrib == userinput::POISSON)
    {
        G.create_random_graph_poisson(n_input.n_nodes, 2, 3, n_input.seed);
    }
    print::print_graph(G);
    simulator sim = simulator(G, n_input.n_drones, n_input.n_drones /* batteries */, n_input.budget, n_input.prob_sigma_prime, n_input.seed);

    assert(sim.check_feasibility());

    for (auto const &v : G.get_vertices())
        std::cout << G.get_coord_x(v) << " " << G.get_coord_y(v) << std::endl;
    std::cout << std::endl;

    std::vector<std::string> algs = {"PRIM", "TOP", "GMAX", "GMIN", "TOP-PRIM", "TOP-GREEDY", "GMAX-ROUND", "TOP 2-apx", "TOP-GMAX-ROUND"};

    for (int i = 0; i < 9; i++)
    {
        if (i == 2 or i == 3 or i == 4 or i == 5 or i == 7 or i == 8)
            continue; // skip some of the algs
        std::vector<std::vector<std::vector<std::pair<int, double>>>> sol = sim.meta_algorithm(i);

        std::cout << std::endl
                  << algs[i] << " ALGORITHM" << std::endl;

        print::print_solution(sol);

        for (size_t dr = 0; dr < sol.size(); dr++)
        {
            std::cout << std::endl
                      << "Drone " << dr << std::endl;
            for (size_t round = 0; round < sol[dr].size(); round++)
            {
                for (size_t node = 0; node < sol[dr][round].size(); node++)
                {
                    std::cout << G.get_coord_x(sol[dr][round][node].first) << " " << G.get_coord_y(sol[dr][round][node].first) << std::endl;
                }
                break;
            }
        }
    }
    return;
}

int main(int argc, char **argv)
{
    input n_input = userinput::read_user_input(argc, argv);

    if (n_input.experiment == 1) // --simulation generate-graph
    {
        print_graph_to_file(n_input);
    }
    else if (n_input.experiment == 2) // --simulation test-batteries
    {
        batteries_greedy(n_input);
    }
    else if (n_input.experiment == 0) // --simulation full-simulation
    {
        full_simulation(n_input);
    }
    else if (n_input.experiment == 3) // --simulation top-comparison
    {
        top_comparison(n_input);
    }
    else if (n_input.experiment == 4) // --simulation print-cycles
    {
        print_cycles(n_input);
    }

    return 0;
}