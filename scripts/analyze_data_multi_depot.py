import numpy
import sys
import os
from collections import defaultdict

from texstrings import *

algs = []
colors = defaultdict(dict)
plots = []

d_table_n_cycles = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))
d_table_time_cycles_avg = defaultdict(
    lambda: defaultdict(lambda: defaultdict(list)))
d_table_time_cycles_min = defaultdict(
    lambda: defaultdict(lambda: defaultdict(list)))

d_fun_1 = defaultdict(lambda: defaultdict(
    lambda: defaultdict(list)))
d_fun_2 = defaultdict(lambda: defaultdict(
    lambda: defaultdict(list)))
d_fun_3 = defaultdict(lambda: defaultdict(
    lambda: defaultdict(list)))

plot_time = defaultdict(lambda: defaultdict(
    lambda: defaultdict(list)))

time_graphs = defaultdict(lambda: defaultdict(
    lambda: defaultdict(list)))

whichcolor = 0
for filename in os.listdir("data/output"):
    if ".csv" in filename:
        _temp_alg_name = filename.replace(".csv", "")
        algs.append(_temp_alg_name)
        colors[_temp_alg_name] = color[whichcolor]
        whichcolor += 1

# hardcoding the colors

colors = {"greedy": "red", "kim": "blue"}

################################
### COLLECT DATA FROM FILES ####
################################

for alg in algs:
    with open("data/output/"+alg + ".csv") as fIn:
        for line in fIn:
            graph_file, drone_file, n, d, q, s, fun_1, fun_2, fun_3, time, n_cycles, avg_time_cycles, min_time_cycle = line.strip().split(",")

            d_fun_1[int(d)][alg][int(n)
                                 ].append(float(fun_1.strip()))
            d_fun_2[int(d)][alg][int(n)
                                 ].append(float(fun_2.strip()))
            d_fun_3[int(d)][alg][int(n)
                                 ].append(float(fun_3.strip()))

            plot_time[int(d)][alg][int(n)
                                   ].append(float(time.strip())/(float(n_cycles)/float(q)))

            # if int(q) == 5:
            #     d_table_n_cycles[alg][int(n)][str(p)].append(float(n_cycles))

################################
### OBJECTIVE FUNCTION PLOTS ###
################################

for depots in d_fun_1.keys():
    f = open("data/plots/d" +
             str(depots) + ".tex", "w+")
    f2 = open("data/plots/ct_d" +
              str(depots) + ".tex", "w+")
    f.write(plot_file_1)
    f2.write(plot_file_1)
    f.write("title={d" + str(depots) + "},")
    f2.write("title={d" + str(depots) + "},")
    f.write(plot_file_3)
    f2.write(plot_file_3)
    f.write("ylabel={\\(wL^I\\)},")

    f2.write("ylabel={\\(ct\\)},")
    f.write(plot_file_4)
    f2.write(plot_file_4)

    for alg in algs:
        f.write("\\addplot+["+colors[alg])

        f.write(plot_file_5)
        for n_nodes in d_fun_1[depots][alg].keys():
            arr = numpy.array(d_fun_1[depots][alg][n_nodes])
            avg = numpy.mean(arr, axis=0)
            std = numpy.std(arr, axis=0)
            f.write(str(n_nodes) +
                    " {:.2f} {:.2f}".format(avg, std)+"\n")
        f.write("};\\addlegendentry{"+alg+"};\n\n")
        f2.write("\\addplot+["+colors[alg])
        f2.write(plot_file_5)
        for n_nodes in d_fun_3[depots][alg].keys():
            arr = numpy.array(d_fun_3[depots][alg][n_nodes])
            avg = numpy.mean(arr, axis=0)
            std = numpy.std(arr, axis=0)
            f2.write(str(n_nodes)+" {:.2f} {:.2f}".format(avg, std)+"\n")
        f2.write("};\\addlegendentry{"+alg+"};\n\n")

    f.write(plot_file_6)
    f2.write(plot_file_6)
    f.close()
    f2.close()
    plots.append(f.name)
    plots.append(f2.name)

#############################
### COMPUTATION TIME ########
#############################

for depots in plot_time.keys():
    f = open("data/plots/time_d" +
             str(depots) + ".tex", "w+")
    f.write(plot_file_1)
    f.write("title={Execution time (average per round) \\(d" +
            str(depots) + ")},")
    f.write(plot_file_3)
    f.write("ylabel={\\(\\mu s\\)},")
    f.write(plot_file_4)

    for alg in algs:
        f.write("\\addplot+["+colors[alg])
        f.write(plot_file_5)
        for n_nodes in plot_time[depots][alg].keys():
            arr = numpy.array(plot_time[depots][alg][n_nodes])
            avg = numpy.mean(arr, axis=0)
            std = numpy.std(arr, axis=0)
            f.write(str(n_nodes) +
                    " {:.2f} {:.2f}".format(avg, std)+"\n")
        f.write("};\\addlegendentry{"+alg+"};\n\n")

    f.write(plot_file_6)
    f.close()
    # plots.append(f.name)

#############################
## CREATION MAIN DOCUMENT ###
#############################

simfile = open("data/plots/simulations_data.tex", "w+")
simfile.write(document_part_1)
i = 1
plots.sort()
simfile.write(start_figure)
for plot in plots:
    simfile.write(
        "\\subfloat[]{\n\\includestandalone[scale=1.2]{" + (plot.replace("data/", "")).replace(".tex", "") + "}\n}\n")
    if i % 2 == 0:
        simfile.write(end_figure)
        simfile.write(start_figure)
    i += 1
simfile.write(end_figure)
