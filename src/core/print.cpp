#include "print.h"

void print::print_solution(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol)
{
    std::cout << "**** SOLUTION ***" << std::endl;
    for (size_t i = 0; i < sol.size(); i++)
    {
        std::cout << "Drone " << i << std::endl;
        for (size_t j = 0; j < sol[i].size(); j++)
        {
            std::cout << "- cycle " << j << ": ";
            for (size_t k = 0; k < sol[i][j].size(); k++)
            {
                std::cout << sol[i][j][k].first << " ";
            }
            std::cout << std::endl;
        }
    }
    std::cout << "*****************" << std::endl;
}

void print::print_e_solution(graph G, std::vector<std::vector<std::vector<std::pair<int, double>>>> sol, std::map<int, int> sigma_prime_prob)
{
    std::cout << "**** SOLUTION ***" << std::endl;
    for (size_t i = 0; i < sol.size(); i++)
    {
        std::cout << "Drone " << i << std::endl;
        for (size_t j = 0; j < sol[i].size(); j++)
        {
            std::cout << "- cycle " << j << ": ";
            for (size_t k = 0; k < sol[i][j].size(); k++)
            {
                std::cout << sol[i][j][k].first << " ";
            }
            std::cout << "\tb: " << utilities::cost_budget_sequence(G, sol[i][j], sigma_prime_prob) << std::endl;
        }
    }
    std::cout << "*****************" << std::endl;
}

void print::print_cycle_sol(std::vector<std::pair<int, double>> _cycle)
{
    if (_cycle.size() > 1)
    {
        for (auto &v : _cycle)
            std::cout << v.first << " ";
        std::cout << std::endl;
    }
}

void print::print_set(std::unordered_set<int> _set)
{
    for (auto &i : _set)
        std::cout << i << " ";
    std::cout << std::endl;
}

template <typename T>
void print::print_vector(const T &t)
{
    std::copy(t.cbegin(), t.cend(), std::ostream_iterator<typename T::value_type>(std::cout, " "));
    std::cout << std::endl;
}
template void print::print_vector<std::vector<int>>(const std::vector<int> &);
template void print::print_vector<std::vector<double>>(const std::vector<double> &);

void print::print_map_int_int(std::map<int, int> _map)
{
    for (auto elem : _map)
        std::cout << elem.first << " : " << elem.second << std::endl;
}

void print::print_graph(graph G)
{
    std::cout << "***** GRAPH *****" << std::endl;
    for (auto const &v : G.get_vertices())
        std::cout << v << ": (" << G.get_coord_x(v) << ", " << G.get_coord_y(v) << ") p:" << G.get_priority_node(v) << " w:" << G.get_weight_node(v) << " w':" << G.get_weight_prime_node(v) << std::endl;
    std::cout << "*****************" << std::endl;
}

void print::print_graph_to_file(graph G, std::string path)
{
    std::ofstream myfile;
    myfile.open(path);

    for (auto const &v : G.get_vertices())
        myfile << G.get_coord_x(v) << ", " << G.get_coord_y(v) << ", " << G.get_priority_node(v) << ", " << G.get_weight_node(v) << ", " << G.get_weight_prime_node(v) << std::endl;
    myfile.close();
}