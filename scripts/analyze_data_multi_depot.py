import numpy
import os
from collections import defaultdict

from texstrings import *

algs = []
colors = defaultdict(dict)
plots = []

d_fun_1 = defaultdict(lambda: defaultdict(
    lambda: defaultdict(lambda: defaultdict(list))))
d_fun_2 = defaultdict(lambda: defaultdict(
    lambda: defaultdict(lambda: defaultdict(list))))

d_fun_wadd5_1 = defaultdict(lambda: defaultdict(
    lambda: defaultdict(lambda: defaultdict(list))))
d_fun_wadd5_2 = defaultdict(lambda: defaultdict(
    lambda: defaultdict(lambda: defaultdict(list))))

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

            graph_file, drone_file, n, d, q, fun_1, fun_2, time, n_cycles, avg_time_cycles, min_time_cycle = line.strip().split(",")

            if "wadd5" in graph_file:
                d_fun_wadd5_1[int(d)][int(q)][alg][int(n)].append(float(fun_1.strip()))
                d_fun_wadd5_2[int(d)][int(q)][alg][int(n)].append(float(fun_1.strip()))
            else:
                d_fun_1[int(d)][int(q)][alg][int(n)].append(float(fun_1.strip()))
                d_fun_2[int(d)][int(q)][alg][int(n)].append(float(fun_1.strip()))

# ################################
# ### OBJECTIVE FUNCTION PLOTS ###
# ################################

for depots in d_fun_1.keys():
    for drones in d_fun_1[depots].keys():
        f = open("data/plots/ct_d" + str(depots) + "q" + str(drones) + ".tex", "w+")
        f.write(plot_file_1v2)
        f.write("title={ct d" + str(depots)  + "q" + str(drones) + "},")
        f.write(plot_file_3)
        f.write("ylabel={\\(ct\\)},")
        f.write(plot_file_4v2)

        f2 = open("data/plots/tft_d" + str(depots) + "q" + str(drones) + ".tex", "w+")
        f2.write(plot_file_1v2)
        f2.write("title={TFT d" + str(depots)  + "q" + str(drones) + "},")
        f2.write(plot_file_3)
        f2.write("ylabel={\\(TFT\\)},")
        f2.write(plot_file_4v2)

        f3 = open("data/plots/ct_d" + str(depots) + "q" + str(drones) + "_wadd5.tex", "w+")
        f3.write(plot_file_1v2)
        f3.write("title={ct d" + str(depots)  + "q" + str(drones) + "w add 5},")
        f3.write(plot_file_3)
        f3.write("ylabel={\\(ct\\)},")
        f3.write(plot_file_4v2)

        f4 = open("data/plots/tft_d" + str(depots) + "q" + str(drones) + "_wadd5.tex", "w+")
        f4.write(plot_file_1v2)
        f4.write("title={TFT d" + str(depots)  + "q" + str(drones) + "w add 5},")
        f4.write(plot_file_3)
        f4.write("ylabel={\\(TFT\\)},")
        f4.write(plot_file_4v2)

        for alg in algs:
            f.write("\\addplot+["+colors[alg])
            f.write(plot_file_5)

            f2.write("\\addplot+["+colors[alg])
            f2.write(plot_file_5)

            f3.write("\\addplot+["+colors[alg])
            f3.write(plot_file_5)

            f4.write("\\addplot+["+colors[alg])
            f4.write(plot_file_5)

            for n_nodes in d_fun_1[depots][drones][alg].keys():
                arr = numpy.array(d_fun_1[depots][drones][alg][n_nodes])
                avg = numpy.mean(arr, axis=0)
                std = numpy.std(arr, axis=0)
                f.write(str(n_nodes- int(depots)) + " {:.2f} {:.2f}".format(avg, std)+"\n")

                arr = numpy.array(d_fun_2[depots][drones][alg][n_nodes])
                avg = numpy.mean(arr, axis=0)
                std = numpy.std(arr, axis=0)
                f2.write(str(n_nodes- int(depots)) + " {:.2f} {:.2f}".format(avg, std)+"\n")

                arr = numpy.array(d_fun_wadd5_2[depots][drones][alg][n_nodes])
                avg = numpy.mean(arr, axis=0)
                std = numpy.std(arr, axis=0)
                f3.write(str(n_nodes- int(depots)) + " {:.2f} {:.2f}".format(avg, std)+"\n")

                arr = numpy.array(d_fun_wadd5_2[depots][drones][alg][n_nodes])
                avg = numpy.mean(arr, axis=0)
                std = numpy.std(arr, axis=0)
                f4.write(str(n_nodes- int(depots)) + " {:.2f} {:.2f}".format(avg, std)+"\n")

            f.write("};\\addlegendentry{"+alg+"};\n\n")
            f2.write("};\\addlegendentry{"+alg+"};\n\n")
            f3.write("};\\addlegendentry{"+alg+"};\n\n")
            f4.write("};\\addlegendentry{"+alg+"};\n\n")
        f.write(plot_file_6)
        f.close()

        f2.write(plot_file_6)
        f2.close()

        f3.write(plot_file_6)
        f3.close()

        f4.write(plot_file_6)
        f4.close()

        plots.append(f.name)
        plots.append(f2.name)
        plots.append(f3.name)
        plots.append(f4.name)

# #############################
# ### COMPUTATION TIME ########
# #############################

# for depots in plot_time.keys():
#     f = open("data/plots/time_d" +
#              str(depots) + ".tex", "w+")
#     f.write(plot_file_1)
#     f.write("title={Execution time (average per round) \\(d" +
#             str(depots) + ")},")
#     f.write(plot_file_3)
#     f.write("ylabel={\\(\\mu s\\)},")
#     f.write(plot_file_4)

#     for alg in algs:
#         f.write("\\addplot+["+colors[alg])
#         f.write(plot_file_5)
#         for n_nodes in plot_time[depots][alg].keys():
#             arr = numpy.array(plot_time[depots][alg][n_nodes])
#             avg = numpy.mean(arr, axis=0)
#             std = numpy.std(arr, axis=0)
#             f.write(str(n_nodes) +
#                     " {:.2f} {:.2f}".format(avg, std)+"\n")
#         f.write("};\\addlegendentry{"+alg+"};\n\n")

#     f.write(plot_file_6)
#     f.close()
#     # plots.append(f.name)

# #############################
# ## CREATION MAIN DOCUMENT ###
# #############################

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
