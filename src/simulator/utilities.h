#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include <map>

#include "../core/algo.h"
#include "../core/print.h"

class utilities
{

private:
public:
    utilities();
    ~utilities();

    static std::unordered_set<int> set_difference(std::unordered_set<int>, std::vector<int>);
    static std::vector<int> set_to_tsp(graph G, double budget, std::unordered_set<int> _temp);
    static double cost_budget_sequence(graph G, std::vector<int> sequence, std::map<int, int> sigma_prime_probs);
    static double cost_budget_sequence(graph G, std::vector<std::pair<int, double>> _temp, std::map<int, int> sigma_prime_prob);
    static std::vector<int> check_cycle_sigma_prime_cycle(graph G, std::map<int, int> sigma_prime_prob, long seed, double budget, std::vector<int> cycle);
    static std::vector<int> check_cycle_sigma_prime_cycle(graph G, std::map<int, int> sigma_prime_prob, long seed, double budget, int start, std::vector<int> cycle);
};

#endif // UTILITIES_H