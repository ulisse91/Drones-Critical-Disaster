#include "utilities.h"

void utilities::print_solution(std::vector<std::vector<std::vector<std::pair<int, double>>>> sol)
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

void utilities::print_cycle_sol(std::vector<std::pair<int, double>> _cycle)
{
    for (size_t k = 0; k < _cycle.size(); k++)
    {
        std::cout << _cycle[k].first << " ";
    }
    std::cout << std::endl;
}

void utilities::print_set(std::unordered_set<int> _set)
{
    for (auto &i : _set)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void utilities::print_vector_int(std::vector<int> _vec)
{
    for (size_t i = 0; i < _vec.size(); i++)
    {
        std::cout << _vec[i] << " ";
    }
    std::cout << std::endl;
}

void utilities::print_map_int_int(std::map<int, int> _map)
{
    for (auto elem : _map)
    {
        std::cout << elem.first << " : " << elem.second << std::endl;
    }
}