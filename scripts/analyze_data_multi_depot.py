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
d_time = defaultdict(lambda: defaultdict(
    lambda: defaultdict(lambda: defaultdict(list))))

d_fun_wadd5_1 = defaultdict(lambda: defaultdict(
    lambda: defaultdict(lambda: defaultdict(list))))
d_fun_wadd5_2 = defaultdict(lambda: defaultdict(
    lambda: defaultdict(lambda: defaultdict(list))))
d_time_wadd5 = defaultdict(lambda: defaultdict(
    lambda: defaultdict(lambda: defaultdict(list))))

whichcolor = 0
for filename in os.listdir("data/output"):
    if ".csv" in filename:
        _temp_alg_name = filename.replace(".csv", "")
        algs.append(_temp_alg_name)
        colors[_temp_alg_name] = color[whichcolor]
        whichcolor += 1

# hardcoding the colors anyway :)
colors = {"greedy": "red", "kim": "blue"}

################################
### COLLECT DATA FROM FILES ####
################################

for alg in algs:
    with open("data/output/"+alg + ".csv") as fIn:
        for line in fIn:

            graph_file, drone_file, n, d, q, fun_1, fun_2, time, n_cycles, ad_vg_time_cycles, md_in_time_cycle = line.strip().split(",")

            if "wadd5" in graph_file:
                d_fun_wadd5_1[int(d)][int(q)][alg][int(
                    n)].append(float(fun_1.strip()))
                d_fun_wadd5_2[int(d)][int(q)][alg][int(
                    n)].append(float(fun_2.strip()))
                d_time_wadd5[int(d)][int(q)][alg][int(
                    n)].append(float(time.strip()))
            else:
                d_fun_1[int(d)][int(q)][alg][int(n)].append(
                    float(fun_1.strip()))
                d_fun_2[int(d)][int(q)][alg][int(n)].append(
                    float(fun_2.strip()))

                d_time[int(d)][int(q)][alg][int(n)].append(float(time.strip()))

# ################################
# ### OBJECTIVE FUNCTION PLOTS ###
# ################################

for depots in d_fun_1.keys():
    for drones in d_fun_1[depots].keys():
        f = open("data/plots/ct_d" + str(depots) +
                 "q" + str(drones) + ".tex", "w+")
        f.write(plot_file_1v2)
        f.write("title={ct d" + str(depots) + "q" + str(drones) + "},")
        f.write(plot_file_3)
        f.write("ylabel={\\(ct\\)},")
        f.write(plot_file_4v2)

        f2 = open("data/plots/tft_d" + str(depots) +
                  "q" + str(drones) + ".tex", "w+")
        f2.write(plot_file_1v2)
        f2.write("title={TFT d" + str(depots) + "q" + str(drones) + "},")
        f2.write(plot_file_3)
        f2.write("ylabel={\\(TFT\\)},")
        f2.write(plot_file_4v2)

        f3 = open("data/plots/ct_d" + str(depots) +
                  "q" + str(drones) + "_wadd5.tex", "w+")
        f3.write(plot_file_1v2)
        f3.write("title={ct d" + str(depots) + "q" + str(drones) + "w add 5},")
        f3.write(plot_file_3)
        f3.write("ylabel={\\(ct\\)},")
        f3.write(plot_file_4v2)

        f4 = open("data/plots/tft_d" + str(depots) +
                  "q" + str(drones) + "_wadd5.tex", "w+")
        f4.write(plot_file_1v2)
        f4.write("title={TFT d" + str(depots) +
                 "q" + str(drones) + "w add 5},")
        f4.write(plot_file_3)
        f4.write("ylabel={\\(TFT\\)},")
        f4.write(plot_file_4v2)

        f5 = open("data/plots/time_d" + str(depots) +
                  "q" + str(drones) + ".tex", "w+")
        f5.write(plot_file_1v2)
        f5.write("title={time d" + str(depots) + "q" + str(drones) + "},")
        f5.write(plot_file_3)
        f5.write("ylabel={\\(\mu s\\)},")
        f5.write(plot_file_4v2)

        f6 = open("data/plots/time_d" + str(depots) +
                  "q" + str(drones) + "_wadd5.tex", "w+")
        f6.write(plot_file_1v2)
        f6.write("title={time d" + str(depots) +
                 "q" + str(drones) + "w add 5},")
        f6.write(plot_file_3)
        f6.write("ylabel={\\(\mu s\\)},")
        f6.write(plot_file_4v2)

        for alg in algs:
            f.write("\\addplot+["+colors[alg])
            f.write(plot_file_5)

            f2.write("\\addplot+["+colors[alg])
            f2.write(plot_file_5)

            f3.write("\\addplot+["+colors[alg])
            f3.write(plot_file_5)

            f4.write("\\addplot+["+colors[alg])
            f4.write(plot_file_5)

            f5.write("\\addplot+["+colors[alg])
            f5.write(plot_file_5)

            f6.write("\\addplot+["+colors[alg])
            f6.write(plot_file_5)

            for n_nodes in d_fun_1[depots][drones][alg].keys():
                arr = numpy.array(d_fun_1[depots][drones][alg][n_nodes])
                avg = numpy.mean(arr, axis=0)
                std = numpy.std(arr, axis=0)
                f.write(str(n_nodes - int(depots)) +
                        " {:.2f} {:.2f}".format(avg, std)+"\n")

                arr = numpy.array(d_fun_2[depots][drones][alg][n_nodes])
                avg = numpy.mean(arr, axis=0)
                std = numpy.std(arr, axis=0)
                f2.write(str(n_nodes - int(depots)) +
                         " {:.2f} {:.2f}".format(avg, std)+"\n")

                arr = numpy.array(d_fun_wadd5_1[depots][drones][alg][n_nodes])
                avg = numpy.mean(arr, axis=0)
                std = numpy.std(arr, axis=0)
                f3.write(str(n_nodes - int(depots)) +
                         " {:.2f} {:.2f}".format(avg, std)+"\n")

                arr = numpy.array(d_fun_wadd5_2[depots][drones][alg][n_nodes])
                avg = numpy.mean(arr, axis=0)
                std = numpy.std(arr, axis=0)
                f4.write(str(n_nodes - int(depots)) +
                         " {:.2f} {:.2f}".format(avg, std)+"\n")

                arr = numpy.array(d_time[depots][drones][alg][n_nodes])
                avg = numpy.mean(arr, axis=0)
                std = numpy.std(arr, axis=0)
                f5.write(str(n_nodes - int(depots)) +
                         " {:.2f} {:.2f}".format(avg, std)+"\n")

                arr = numpy.array(d_time_wadd5[depots][drones][alg][n_nodes])
                avg = numpy.mean(arr, axis=0)
                std = numpy.std(arr, axis=0)
                f6.write(str(n_nodes - int(depots)) +
                         " {:.2f} {:.2f}".format(avg, std)+"\n")

            f.write("};\\addlegendentry{"+alg+"};\n\n")
            f2.write("};\\addlegendentry{"+alg+"};\n\n")
            f3.write("};\\addlegendentry{"+alg+"};\n\n")
            f4.write("};\\addlegendentry{"+alg+"};\n\n")
            f5.write("};\\addlegendentry{"+alg+"};\n\n")
            f6.write("};\\addlegendentry{"+alg+"};\n\n")
        f.write(plot_file_6)
        f.close()

        f2.write(plot_file_6)
        f2.close()

        f3.write(plot_file_6)
        f3.close()

        f4.write(plot_file_6)
        f4.close()

        f5.write(plot_file_6)
        f5.close()

        f6.write(plot_file_6)
        f6.close()

        plots.append(f.name)
        plots.append(f2.name)
        plots.append(f3.name)
        plots.append(f4.name)
        plots.append(f5.name)
        plots.append(f6.name)

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
simfile.write(document_part_2)
simfile.close()
