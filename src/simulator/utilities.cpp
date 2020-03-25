#include "utilities.h"

std::unordered_set<int> utilities::set_difference(std::unordered_set<int> main, std::vector<int> minus)
{
    // for (int i = 0; i < minus.size(); i++)
    // {
    //     if (main.find(minus[i]) != main.end())
    //     {
    //         main.erase(main.find(minus[i]));
    //     }
    // }
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

std::vector<int> utilities::check_cycle_sigma_prime_cycle(graph G, std::map<int, int> sigma_prime_prob, long seed, double budget, std::vector<int> cycle)
{
    // print::print_vector_int(cycle);

    std::vector<int> effective_cycle = {cycle[0]};

    std::uniform_real_distribution<double> unif_1(0, 1);
    std::mt19937 re(seed);

    double cost_cycle = 0;
    for (size_t v = 0; v < cycle.size() - 1; ++v)
    {
        // std::cerr << cost_cycle << " " << G.distw(v, v + 1) << " " << sigma_prime_prob[v + 1] * G.get_weight_prime_node(v + 1) << " " << G.distw(v + 1, 0) << std::endl;

        if (cost_cycle + G.distw(cycle[v], cycle[v + 1]) + sigma_prime_prob[cycle[v + 1]] * G.get_weight_prime_node(cycle[v + 1]) + G.distw(cycle[v + 1], 0) < budget)
        {
            cost_cycle += G.distw(cycle[v], cycle[v + 1]) + sigma_prime_prob[cycle[v + 1]] * G.get_weight_prime_node(cycle[v + 1]);
            effective_cycle.push_back(cycle[v + 1]);
        }
        else
        {
            // print::print_vector_int(effective_cycle);
            return effective_cycle;
        }
    }

    // print::print_vector_int(effective_cycle);
    return effective_cycle;
}

std::vector<int> utilities::check_cycle_sigma_prime_cycle(graph G, std::map<int, int> sigma_prime_prob, long seed, double budget, int start, std::vector<int> cycle)
{
    // print::print_vector_int(cycle);

    std::vector<int> effective_cycle = {cycle[0]};

    std::uniform_real_distribution<double> unif_1(0, 1);
    std::mt19937 re(seed);

    double cost_cycle = 0;
    for (size_t v = 0; v < cycle.size() - 1; ++v)
    {
        // std::cerr << cost_cycle << " " << G.distw(v, v + 1) << " " << sigma_prime_prob[v + 1] * G.get_weight_prime_node(v + 1) << " " << G.distw(v + 1, 0) << std::endl;

        if (cost_cycle + G.distw(cycle[v], cycle[v + 1]) + sigma_prime_prob[cycle[v + 1]] * G.get_weight_prime_node(cycle[v + 1]) + G.distw(cycle[v + 1], 0) < budget)
        {
            cost_cycle += G.distw(cycle[v], cycle[v + 1]) + sigma_prime_prob[cycle[v + 1]] * G.get_weight_prime_node(cycle[v + 1]);
            effective_cycle.push_back(cycle[v + 1]);
        }
        else
        {
            // print::print_vector_int(effective_cycle);
            return effective_cycle;
        }
    }

    // print::print_vector_int(effective_cycle);
    return effective_cycle;
}