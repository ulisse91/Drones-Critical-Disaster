#include "simulator_multi_depot.h"

simulator_md::simulator_md(graph _G, std::vector<std::tuple<int, int, double>> _drones, int _n_depots, long _seed)
{
    this->G = _G;
    this->drones = _drones;
    this->n_depots = _n_depots;
    this->seed = _seed;

    // assert(this->budget > 0);
    // assert(this->n_batteries >= this->n_drones);
}

simulator_md::~simulator_md() {}

bool simulator_md::check_feasibility_multi_depot()
{
    for (auto &v : this->G.get_vertices())
    {
        bool temp = false;
        for (std::tuple drone : this->drones)
        {
            temp = temp or (2 * G.distw(std::get<1>(drone), v) <= std::get<2>(drone));
            // std::cout << temp << ": distw(" << v << ", " << std::get<1>(drone) << ") = " << 2 * G.distw(std::get<1>(drone), v) << " | budget: " << std::get<2>(drone) << "\n";
        }
        if (not temp)
            return false;
    }
    return true;
}