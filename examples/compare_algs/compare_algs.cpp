#include <assert.h>

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
    G.create_random_graph(n_input.n_nodes, 0, 3);

    print::print_graph(G);

    simulator sim = simulator(G, n_input.n_drones, n_input.n_drones /* batteries */, n_input.budget);
    assert(sim.check_feasibility());
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol_top = sim.top_based_alg();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol_greedy = sim.greedy_based_alg();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol_prim = sim.prim_based_alg();

    print::print_solution(sol_top);
    std::cout << "fun_cycle: " << sim.evaluate_solution(0, sol_top) << " func_weighted_latency: " << sim.evaluate_solution(1, sol_top) << std::endl
              << std::endl;
    print::print_solution(sol_greedy);
    std::cout << "fun_cycle: " << sim.evaluate_solution(0, sol_greedy) << " func_weighted_latency: " << sim.evaluate_solution(1, sol_greedy) << std::endl
              << std::endl;
    print::print_solution(sol_prim);
    std::cout << "fun_cycle: " << sim.evaluate_solution(0, sol_prim) << " func_weighted_latency: " << sim.evaluate_solution(1, sol_prim) << std::endl
              << std::endl;
    std::cout << std::endl;

    assert(sim.check_solution_feasible(sol_top));
    assert(sim.check_solution_feasible(sol_greedy));
    assert(sim.check_solution_feasible(sol_prim));
    return 0;
}