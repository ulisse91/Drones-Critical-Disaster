#include <assert.h>
#include <chrono> 

#include "../../src/core/user_input.h"
#include "../../src/simulator/simulator.h"

/*
    run example with: (NB: pay attention to relative path wrt where you run the code)
    make examples; ./examples/build/compare_algs -b 5 -d 2 -n 15
*/

int main(int argc, char **argv)
{
    input n_input = userinput::read_user_input(argc, argv);

    graph G = graph(2, 2);
    G.create_random_graph(n_input.n_nodes, 3, 3, n_input.seed);

    print::print_graph(G);

    simulator sim = simulator(G, n_input.n_drones, n_input.n_drones /* batteries */, n_input.budget);

    assert(sim.check_feasibility());

    auto start_t = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol_top = sim.top_based_alg();
    auto stop_t = std::chrono::high_resolution_clock::now();
    std::cout << "TOP BASED ALGORITHM" << std::endl;
    print::print_e_solution(G, sol_top);
    std::cout << "fun_cycle: " << sim.evaluate_solution(0, sol_top) << " func_weighted_latency: " << sim.evaluate_solution(1, sol_top) << " func_completion_time: " << sim.evaluate_solution(2, sol_top) << " | time(ms): " << std::chrono::duration_cast<std::chrono::milliseconds>(stop_t - start_t).count() << std::endl
              << std::endl;
    assert(sim.check_solution_feasible(sol_top) == 1);

    auto start_g = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol_greedy = sim.greedy_based_alg();
    auto stop_g = std::chrono::high_resolution_clock::now();
    std::cout << "GREEDY ALGORITHM" << std::endl;
    print::print_e_solution(G, sol_greedy);
    std::cout << "fun_cycle: " << sim.evaluate_solution(0, sol_greedy) << " func_weighted_latency: " << sim.evaluate_solution(1, sol_greedy) << " func_completion_time: " << sim.evaluate_solution(2, sol_greedy) << " | time(mus): " << std::chrono::duration_cast<std::chrono::microseconds>(stop_g - start_g).count() << std::endl
              << std::endl;
    assert(sim.check_solution_feasible(sol_greedy) == 1);

    auto start_p = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol_prim = sim.prim_based_alg();
    auto stop_p = std::chrono::high_resolution_clock::now();
    std::cout << "PRIM BASED ALGORITHM" << std::endl;
    print::print_e_solution(G, sol_prim);
    std::cout << "fun_cycle: " << sim.evaluate_solution(0, sol_prim) << " func_weighted_latency: " << sim.evaluate_solution(1, sol_prim) << " func_completion_time: " << sim.evaluate_solution(2, sol_prim) << " | time(mus): " << std::chrono::duration_cast<std::chrono::microseconds>(stop_p - start_p).count() << std::endl
              << std::endl;
    std::cout << std::endl;
    assert(sim.check_solution_feasible(sol_prim) == 1);

    auto start_tp = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> top_plus_prim = sim.top_plus_prim();
    auto stop_tp = std::chrono::high_resolution_clock::now();
    std::cout << "TOP+PRIM BASED ALGORITHM" << std::endl;
    print::print_e_solution(G, top_plus_prim);
    std::cout << "fun_cycle: " << sim.evaluate_solution(0, top_plus_prim) << " func_weighted_latency: " << sim.evaluate_solution(1, top_plus_prim) << " func_completion_time: " << sim.evaluate_solution(2, top_plus_prim) << " | time(ms): " << std::chrono::duration_cast<std::chrono::milliseconds>(stop_tp - start_tp).count() << std::endl
              << std::endl;
    std::cout << std::endl;
    assert(sim.check_solution_feasible(top_plus_prim) == 1);

    return 0;
}