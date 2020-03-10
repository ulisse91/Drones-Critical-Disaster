#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <vector>
#include <unordered_set>

class utilities
{

private:
public:
    utilities();
    ~utilities();

    static std::unordered_set<int> set_difference(std::unordered_set<int>, std::vector<int>);

    // print functions
    static void print_solution(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol);
    static void print_set(std::unordered_set<int> _set);
    static void print_vector_int(std::vector<int> _vec);
};

#endif // UTILITIES_H