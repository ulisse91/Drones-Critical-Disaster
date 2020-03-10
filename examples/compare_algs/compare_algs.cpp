#include "../../src/core/user_input.h"
#include "../../src/core/simulator.h"

/*
    run example with: (NB: pay attention to relative path wrt where you run the code)
    make examples; ./examples/build/compare_algs -b 5 -d 2 -n 15
*/

int main(int argc, char **argv)
{
    input n_input = userinput::read_user_input(argc, argv);

    graph G = graph(2, 2);
    //G.read_graph_from_file(n_input.graph_file);
    G.create_random_graph(n_input.n_nodes, 0, 3);

    G.print_graph();

    simulator sim = simulator(G, n_input.n_drones, n_input.n_drones /* batteries */, n_input.budget);
    std::cout << "Graph feasible wrt budget: " << sim.check_feasibility() << std::endl;
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol_top = sim.top_path_BB();
    std::vector<std::vector<std::vector<std::pair<int, double>>>> sol_greedy = sim.greedy_algorithm();

    utilities::print_solution(sol_top);
    std::cout << "fun_cycle: " << sim.evaluate_solution(0, sol_top) << " func_weighted_latency: " << sim.evaluate_solution(1, sol_top) << std::endl;
    utilities::print_solution(sol_greedy);
    std::cout << "fun_cycle: " << sim.evaluate_solution(0, sol_greedy) << " func_weighted_latency: " << sim.evaluate_solution(1, sol_greedy) << std::endl;
    std::cout << std::endl;
    return 0;
}