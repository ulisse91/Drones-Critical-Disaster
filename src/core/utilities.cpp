#include "utilities.h"

void utilities::print_solution(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol)
{
    for (size_t i = 0; i < sol.size(); i++)
    {
        std::cerr << "Drone " << i << std::endl;
        for (size_t j = 0; j < sol[i].size(); j++)
        {
            std::cerr << "cycle " << j << ": ";
            for (size_t k = 0; k < sol[i][j].size(); k++)
            {
                std::cerr << sol[i][j][k].first << " ";
            }
            std::cerr << std::endl;
        }
        std::cerr << std::endl;
    }
}

std::unordered_set<int> utilities::set_difference(std::unordered_set<int> main, std::vector<int> minus)
{
    for (int i = 0; i < minus.size(); i++)
    {
        if (main.find(minus[i]) != main.end())
        {
            main.erase(main.find(minus[i]));
        }
    }
    return main;
}

void utilities::print_set(std::unordered_set<int> _set)
{
    for (auto &i : _set)
    {
        std::cerr << i << " ";
    }
    std::cerr << std::endl;
}

void utilities::print_vector_int(std::vector<int> _vec)
{
    for (size_t i = 0; i < _vec.size(); i++)
    {
        std::cerr << _vec[i] << " ";
    }
    std::cerr << std::endl;
}