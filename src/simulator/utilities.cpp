#include "utilities.h"

std::unordered_set<int> utilities::set_difference(std::unordered_set<int> main, std::vector<int> minus)
{
    for (auto const &i : minus)
    {
        if (main.find(i) != main.end())
        {
            main.erase(main.find(i));
        }
    }
    return main;
}

std::vector<int> utilities::set_to_tsp(graph G, double budget, std::unordered_set<int> _temp)
{
    graph G_prime = graph(G.get_area_x(), G.get_area_y());
    for (const int &i : _temp)
        G_prime.add_node(i, G.get_coord_x(i), G.get_coord_y(i), G.get_weight_node(i), G.get_priority_node(i), G.get_weight_prime_node(i));
    // print::print_graph(G_prime);
    return algo::find_TSP(G_prime, budget, 0, algo::primMST(G_prime, {0}, budget));
}

double utilities::cost_budget_sequence(graph G, std::vector<int> sequence, std::map<int, int> sigma_prime_probs)
{
    double sum_of_elems = 0;
    if (sequence.size() > 1)
    {
        for (size_t i = 0; i < sequence.size() - 1; i++)
        {
            sum_of_elems += G.distw(sequence[i], sequence[i + 1]) + sigma_prime_probs[sequence[i + 1]] * G.get_weight_prime_node(sequence[i + 1]);
            // std::cout << sequence[i]<< " -> " << sequence[i + 1] << " = " << G.distw(sequence[i], sequence[i + 1]) << " | " << sum_of_elems << "\n";
        }
    }
    return sum_of_elems;
}

double utilities::cost_budget_sequence(graph G, std::vector<int> sequence)
{
    double sum_of_elems = 0;
    if (sequence.size() > 1)
    {
        for (size_t i = 0; i < sequence.size() - 1; i++)
        {
            sum_of_elems += G.distw(sequence[i], sequence[i + 1]);
            // std::cout << sequence[i]<< " -> " << sequence[i + 1] << " = " << G.distw(sequence[i], sequence[i + 1]) << " | " << sum_of_elems << "\n";
        }
    }
    return sum_of_elems;
}

double utilities::cost_budget_sequence(graph G, std::vector<std::pair<int, double>> _temp, std::map<int, int> sigma_prime_probs)
{
    double sum_of_elems = 0;
    if (_temp.size() > 1)
    {
        for (size_t i = 0; i < _temp.size() - 1; i++)
        {
            sum_of_elems += sigma_prime_probs[_temp[i + 1].first] * G.get_weight_prime_node(_temp[i + 1].first) + G.distw(_temp[i].first, _temp[i + 1].first);
        }
    }
    return sum_of_elems;
}