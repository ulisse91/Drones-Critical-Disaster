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

double utilities::cost_budget_sequence(graph G, std::vector<std::pair<int, double>> _cycle, std::map<int, int> sigma_prime_probs)
{
    double sum_of_elems = 0;
    if (_cycle.size() > 1)
    {
        for (size_t i = 0; i < _cycle.size() - 1; i++)
        {
            sum_of_elems += sigma_prime_probs[_cycle[i + 1].first] * G.get_weight_prime_node(_cycle[i + 1].first) + G.distw(_cycle[i].first, _cycle[i + 1].first);
        }
    }
    return sum_of_elems;
}

void utilities::clean_sol(std::vector<std::vector<int>> &_temp)
{
    for (auto &cycle : _temp)
    {
        auto end = cycle.end();
        for (auto it = cycle.begin(); it != end; ++it)
        {
            end = std::remove(it + 1, end, *it);
        }
        cycle.erase(end, cycle.end());
    }
}

void utilities::clean_sol_full(std::vector<std::vector<std::vector<std::pair<int, double>>>> &sol)
{
    for (auto &drone : sol)
    {
        for (auto it = drone.begin(); it != drone.end();)
        {
            if (it->size() == 2)
            {
                it = drone.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

std::vector<double> utilities::stat_sol(graph G, std::vector<std::vector<std::vector<std::pair<int, double>>>> sol, std::map<int, int> sigma_prime_prob, double budget)
{
    int tot_n_cycles = 0;
    double avg_time_cycle = 0;
    double min_time_cycle = budget;

    for (size_t i = 0; i < sol.size(); i++)
    {
        tot_n_cycles += sol[i].size();
        for (int j = 0; j < (int)sol[i].size() - 1; j++)
        {
            double t_cycle = utilities::cost_budget_sequence(G, sol[i][j], sigma_prime_prob);
            avg_time_cycle += t_cycle;
            if (t_cycle < min_time_cycle)
            {
                min_time_cycle = t_cycle;
            }
        }
    }
    return std::vector<double>{(double)tot_n_cycles, avg_time_cycle / (tot_n_cycles - sol.size()), min_time_cycle};
}