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

/*
    All'interno di ogni file ogni riga indica un nodo differente.
    Ogni nodo, i.e., ogni riga ha 7 campi (separati da virgola):
    - id = id del nodo
    - x, y = coordinate nello spazio del nodo
    - priority = priorita' del nodo [le priorita' utilizzate sono 1,2,3]
    - sigma = peso del nodo
    - sigmaprime = peso del nodo da considerare se e solo se il campo p e' uguale a 1
    - p = valore booleano che indica se nell'istanza si deve considerare il valore sigma'
    NB: i valori di sigmaprime e p devono essere considerati solo nel secondo scenario

    Il primo nodo rappresenta il depot e avra' sempre tutti i campi a 0.
    Il depot/base e' un nodo con id=0, si trova in coordinate (0,0) e ha sempre valori nulli di priorita'/peso/p

    Esempio:

    0, 0, 0, 0, 0, 0, 0
    # il depot e si trova sempre in posizione (0,0)

    1, 0.939447, 0.4357, 3, 1.82, 2.34, 1
    # nodo con id=1
    # si trova in posizione (0.939447, 0.4357)
    # ha priorita' 3
    # il tempo di sorvolo scenario I = sigma = 1.82
    # il tempo di sorvolo scenario II = sigma + sigmaprime * p = 4.16

    2, 0.256719, 0.553081, 2, 1.62, 2.85, 0
    # nodo con id=2
    # si trova in posizione (0.256719, 0.553081)
    # ha priorita' 2
    # il tempo di sorvolo scenario I = sigma = 1.62
    # il tempo di sorvolo scenario II = sigma + sigmaprime * p = 1.62
*/
void print::print_graph_to_file(graph G, std::map<int, int> sigma_prime_probs, std::string path)
{
    std::ofstream myfile;
    myfile.open(path);
    myfile << "id, x, y, priority, sigma, sigmaprime, p" << std::endl;

    // print_map_int_int(sigma_prime_probs);

    for (auto const &v : G.get_vertices())
        myfile << v << ", " << G.get_coord_x(v) << ", " << G.get_coord_y(v) << ", " << G.get_priority_node(v) << ", " << G.get_weight_node(v) << ", " << G.get_weight_prime_node(v) << ", " << sigma_prime_probs[v] << std::endl;
    myfile.close();
}

void print::print_graph_to_file_multi_depots(graph G, int number_of_depots, std::string path)
{
    std::ofstream myfile;
    myfile.open(path);

    myfile << "D " << number_of_depots << std::endl;
    myfile << "id, x, y, sigma" << std::endl;

    // print_map_int_int(sigma_prime_probs);

    for (auto const &v : G.get_vertices())
        myfile << v << ", " << G.get_coord_x(v) << ", " << G.get_coord_y(v) << ", " << G.get_weight_node(v) << std::endl;
    myfile.close();
}

void print::print_drones_to_file_multi_depots(std::vector<std::tuple<int, int, double>> drones, std::string path)
{
    std::ofstream myfile;
    myfile.open(path);
    myfile << "U " << drones.size() << std::endl;
    myfile << "id, depot_id, budget" << std::endl;

    // print_map_int_int(sigma_prime_probs);

    for (const auto &i : drones)
        myfile << std::get<0>(i) << ", " << std::get<1>(i) << ", " << std::get<2>(i) << std::endl;
    myfile.close();
}

void print::print_drones(std::vector<std::tuple<int, int, double>> drones)
{
    std::cout << "***** DRONES *****" << std::endl;
    for (const auto &i : drones)
        std::cout << std::get<0>(i) << ": d " << std::get<1>(i) << " b " << std::get<2>(i) << std::endl;
    std::cout << "*****************" << std::endl;
}