#ifndef USERINPUT_H
#define USERINPUT_H

#include <iostream>
#include <time.h>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

struct input
{
    /// Default values
    int n_nodes = 10;
    int n_drones = 1;
    int n_batteries = n_drones;
    double budget = 1;
    std::string graph_file = "";

    input(int _n_nodes, int _n_drones, int _n_batteries, double _budget, std::string _graph_file) : n_nodes(_n_nodes), n_drones(_n_drones), n_batteries(_n_batteries), budget(_budget), graph_file(_graph_file) {}
    input() = default;
};

class userinput
{

private:
public:
    userinput();
    ~userinput();

    static input read_user_input(int argc, char **argv);
};

#endif // USERINPUT_H