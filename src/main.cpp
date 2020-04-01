#include <assert.h>
#include <chrono>

#include "core/user_input.h"
#include "simulator/simulator.h"

int main(int argc, char **argv)
{
    input n_input = userinput::read_user_input(argc, argv);

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

    auto start_t2 = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol_top_meta = sim.meta_algorithm(1);
    auto stop_t2 = std::chrono::high_resolution_clock::now();
    std::cout << "TOP BASED ALGORITHM" << std::endl;
    print::print_e_solution(G, sol_top_meta, sim.sigma_prime_probs);
    std::cout << "fun_cycle: " << sim.evaluate_solution(0, sol_top_meta) << " func_weighted_latency: " << sim.evaluate_solution(1, sol_top_meta) << " func_completion_time: " << sim.evaluate_solution(2, sol_top_meta) << " | time(ms): " << std::chrono::duration_cast<std::chrono::milliseconds>(stop_t2 - start_t2).count() << std::endl;
    print::print_vector(sim.completion_time_priorities(sol_top_meta));

    std::cout << std::endl;
    assert(sim.check_solution_feasible(sol_top_meta) == 1);

    auto start_gmax2 = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol_greedy_max_meta = sim.meta_algorithm(2);
    auto stop_gmax2 = std::chrono::high_resolution_clock::now();
    std::cout << "GREEDY-MAX ALGORITHM" << std::endl;
    print::print_e_solution(G, sol_greedy_max_meta, sim.sigma_prime_probs);
    std::cout << "fun_cycle: " << sim.evaluate_solution(0, sol_greedy_max_meta) << " func_weighted_latency: " << sim.evaluate_solution(1, sol_greedy_max_meta) << " func_completion_time: " << sim.evaluate_solution(2, sol_greedy_max_meta) << " | time(mus): " << std::chrono::duration_cast<std::chrono::microseconds>(stop_gmax2 - start_gmax2).count() << std::endl
              << std::endl;
    assert(sim.check_solution_feasible(sol_greedy_max_meta) == 1);

    auto start_gmin2 = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol_greedy_min_meta = sim.meta_algorithm(3);
    auto stop_gmin2 = std::chrono::high_resolution_clock::now();
    std::cout << "GREEDY-MIN ALGORITHM" << std::endl;
    print::print_e_solution(G, sol_greedy_min_meta, sim.sigma_prime_probs);
    std::cout << "fun_cycle: " << sim.evaluate_solution(0, sol_greedy_min_meta) << " func_weighted_latency: " << sim.evaluate_solution(1, sol_greedy_min_meta) << " func_completion_time: " << sim.evaluate_solution(2, sol_greedy_min_meta) << " | time(mus): " << std::chrono::duration_cast<std::chrono::microseconds>(stop_gmin2 - start_gmin2).count() << std::endl
              << std::endl;
    assert(sim.check_solution_feasible(sol_greedy_min_meta) == 1);

    auto start_p = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol_prim = sim.meta_algorithm(0);
    auto stop_p = std::chrono::high_resolution_clock::now();
    std::cout << "PRIM BASED ALGORITHM" << std::endl;
    print::print_e_solution(G, sol_prim, sim.sigma_prime_probs);
    std::cout << "fun_cycle: " << sim.evaluate_solution(0, sol_prim) << " func_weighted_latency: " << sim.evaluate_solution(1, sol_prim) << " func_completion_time: " << sim.evaluate_solution(2, sol_prim) << " | time(mus): " << std::chrono::duration_cast<std::chrono::microseconds>(stop_p - start_p).count() << std::endl
              << std::endl;
    std::cout << std::endl;
    assert(sim.check_solution_feasible(sol_prim) == 1);

    auto start_tp2 = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> top_plus_prim_meta = sim.meta_algorithm(4);
    auto stop_tp2 = std::chrono::high_resolution_clock::now();
    std::cout << "TOP+PRIM BASED ALGORITHM" << std::endl;
    print::print_e_solution(G, top_plus_prim_meta, sim.sigma_prime_probs);
    std::cout << "fun_cycle: " << sim.evaluate_solution(0, top_plus_prim_meta) << " func_weighted_latency: " << sim.evaluate_solution(1, top_plus_prim_meta) << " func_completion_time: " << sim.evaluate_solution(2, top_plus_prim_meta) << " | time(ms): " << std::chrono::duration_cast<std::chrono::milliseconds>(stop_tp2 - start_tp2).count() << std::endl
              << std::endl;
    std::cout << std::endl;
    assert(sim.check_solution_feasible(top_plus_prim_meta) == 1);

    return 0;
}