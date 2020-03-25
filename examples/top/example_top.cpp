#include <assert.h>

#include "../../src/core/user_input.h"
#include "../../src/simulator/simulator.h"

/*
    run example with: (NB: pay attention to relative path wrt where you run the code)
    ./example_top -b 3.2 -d 2 -f data/graph/test_primMST.csv
*/

int main(int argc, char **argv)
{
    input n_input = userinput::read_user_input(argc, argv);

    graph G = graph(2, 1);
    G.read_graph_from_file(n_input.graph_file);

    simulator sim = simulator(G, n_input.n_drones, n_input.n_drones /* batteries */, n_input.budget, n_input.prob_sigma_prime, n_input.seed);
    assert(sim.check_feasibility());
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol = sim.top_based_alg();

    print::print_solution(sol);

    std::cout << "fun_cycle: " << sim.evaluate_solution(0, sol) << " func_weighted_latency: " << sim.evaluate_solution(1, sol) << std::endl;
    std::cout << std::endl;

    assert(sim.check_solution_feasible(sol));
    return 0;
}