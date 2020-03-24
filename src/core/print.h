#ifndef PRINT_H
#define PRINT_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include <map>

#include "graph.h"
#include "../simulator/utilities.h"

class print
{

private:
public:
    print();
    ~print();

    // print functions
    static void print_solution(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    static void print_set(std::unordered_set<int> _set);
    static void print_vector_int(std::vector<int> _vec);
    static void print_map_int_int(std::map<int, int> _map);
    static void print_cycle_sol(std::vector<std::pair<int, double>> _cycle);
    static void print_graph(graph G);
    static void print_e_solution(graph G, std::vector<std::vector<std::vector<std::pair<int, double>>>> sol, std::map<int, int> sigma_prime_prob);
    static void print_graph_to_file(graph G, std::string path);
};

#endif // PRINT_H