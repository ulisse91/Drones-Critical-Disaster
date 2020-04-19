import numpy
import sys
import os
from collections import defaultdict

from texstrings import *

algs = []
distrib = sys.argv[1]
plots = []

d_table_n_cycles = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))
d_table_time_cycles_avg = defaultdict(
    lambda: defaultdict(lambda: defaultdict(list)))
d_table_time_cycles_min = defaultdict(
    lambda: defaultdict(lambda: defaultdict(list)))

d_fun_1 = defaultdict(lambda: defaultdict(
    lambda: defaultdict(lambda: defaultdict(list))))
d_fun_2 = defaultdict(lambda: defaultdict(
    lambda: defaultdict(lambda: defaultdict(list))))
d_fun_3 = defaultdict(lambda: defaultdict(
    lambda: defaultdict(lambda: defaultdict(list))))

plot_time = defaultdict(lambda: defaultdict(
    lambda: defaultdict(lambda: defaultdict(list))))

time_graphs = defaultdict(lambda: defaultdict(
    lambda: defaultdict(lambda: defaultdict(list))))

for filename in os.listdir("data/output"):
    if distrib+".csv" in filename:
        _temp_alg_name = filename.replace(".csv", "").replace(
            "_"+distrib, "")
        algs.append(_temp_alg_name)

################################
### COLLECT DATA FROM FILES ####
################################

for alg in algs:
    with open("data/output/"+alg + "_" + str(distrib) + ".csv") as fIn:
        for line in fIn:
            n, b, q, p, s, fun_1, fun_2, fun_3, ct_pmax, ct_pmed, ct_pmin, time, n_cycles, avg_time_cycles, min_time_cycle = line.strip().split(",")

            d_fun_1[str(int(float(p)*100))][alg][int(q)
                                                 ][int(n)].append(float(fun_1.strip()))
            d_fun_2[str(int(float(p)*100))][alg][int(q)
                                                 ][int(n)].append(float(fun_2.strip()))
            d_fun_3[str(int(float(p)*100))][alg][int(q)
                                                 ][int(n)].append(float(fun_3.strip()))
            plot_time[str(int(float(p)*100))][alg][int(q)][int(n)
                                                           ].append(float(time.strip())/(float(n_cycles)/float(q)))

            if int(q) == 4:
                time_graphs[str(n)][str(int(float(p)*100))
                                    ]["ct_pmax"][alg].append(float(ct_pmax))
                time_graphs[str(n)][str(int(float(p)*100))
                                    ]["ct_pmed"][alg].append(float(ct_pmed))
                time_graphs[str(n)][str(int(float(p)*100))
                                    ]["ct_pmin"][alg].append(float(ct_pmin))

            if float(p) == 0:
                d_table_time_cycles_avg[int(q)][int(n)][alg].append(
                    float(avg_time_cycles.strip()))
                d_table_time_cycles_min[int(q)][int(n)][alg].append(
                    float(min_time_cycle.strip()))

            if int(q) == 2:
                d_table_n_cycles[alg][int(n)][str(p)].append(float(n_cycles))

################################
### OBJECTIVE FUNCTION PLOTS ###
################################

for pv in range(0, 125, 25):
    pvalue = pv/100.0
    for drones in range(2, 8, 2):
        f = open("data/plots/q"+str(drones)+"-p" +
                 str(int(pvalue*100)) + "_" + str(distrib) + ".tex", "w+")
        f2 = open("data/plots/ct_q"+str(drones)+"-p" +
                  str(int(pvalue*100)) + "_" + str(distrib) + ".tex", "w+")
        f.write(plot_file_1)
        f2.write(plot_file_1)
        f.write("title={\\(q="+str(drones)+", B=15, p=" +
                str(pvalue)+"\\) (" + distrib + ")},")
        f2.write("title={\\(q="+str(drones)+", B=15, p=" +
                 str(pvalue)+"\\) (" + distrib + ")},")
        f.write(plot_file_3)
        f2.write(plot_file_3)
        if pvalue == 0:
            f.write("ylabel={\\(wL^I\\)},")
        else:
            f.write("ylabel={\\(wL^{II}\\)},")
        f2.write("ylabel={\\(ct\\)},")
        f.write(plot_file_4)
        f2.write(plot_file_4)

        whichcolor = 0
        for alg in algs:
            f.write("\\addplot+["+color[whichcolor])
            whichcolor = (whichcolor + 1) % len(algs)
            f.write(plot_file_5)
            if pvalue == 0:
                for n_nodes in d_fun_1[str(pv)][alg][drones].keys():
                    arr = numpy.array(d_fun_1[str(pv)][alg][drones][n_nodes])
                    avg = numpy.mean(arr, axis=0)
                    std = numpy.std(arr, axis=0)
                    f.write(str(n_nodes) +
                            " {:.2f} {:.2f}".format(avg, std)+"\n")
                f.write("};\\addlegendentry{"+alg+"};\n\n")
            else:
                for n_nodes in d_fun_2[str(pv)][alg][drones].keys():
                    arr = numpy.array(d_fun_2[str(pv)][alg][drones][n_nodes])
                    avg = numpy.mean(arr, axis=0)
                    std = numpy.std(arr, axis=0)
                    f.write(str(n_nodes) +
                            " {:.2f} {:.2f}".format(avg, std)+"\n")
                f.write("};\\addlegendentry{"+alg+"};\n\n")
            f2.write("\\addplot+["+color[whichcolor])
            f2.write(plot_file_5)
            for n_nodes in d_fun_3[str(pv)][alg][drones].keys():
                arr = numpy.array(d_fun_3[str(pv)][alg][drones][n_nodes])
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

# for pv in range(0, 125, 25): #
#     pvalue = pv/100.0
# for drones in range(2, 8, 2):
pv = 100
pvalue = pv/100.0
drones = 4
f = open("data/plots/time_q"+str(drones)+"-p" +
         str(int(pvalue*100)) + "_" + str(distrib) + ".tex", "w+")
f.write(plot_file_1)
f.write("title={Execution time (average per round) \\(q=" +
        str(drones)+", B=15, p="+str(pvalue)+"\\) (" + distrib + ")},")
f.write(plot_file_3)
f.write("ylabel={\\(\\mu s\\)},")
f.write(plot_file_4)

whichcolor = 0
for alg in algs:
    f.write("\\addplot+["+color[whichcolor])
    whichcolor = (whichcolor + 1) % len(algs)
    f.write(plot_file_5)
    for n_nodes in plot_time[str(pv)][alg][drones].keys():
        arr = numpy.array(plot_time[str(pv)][alg][drones][n_nodes])
        avg = numpy.mean(arr, axis=0)
        std = numpy.std(arr, axis=0)
        f.write(str(n_nodes) +
                " {:.2f} {:.2f}".format(avg, std)+"\n")
    f.write("};\\addlegendentry{"+alg+"};\n\n")

f.write(plot_file_6)
f.close()
plots.append(f.name)

#############################
### GRAPHS PRIORITY TIME ####
#############################

drones = 4
for pv in [0, 50, 100]:
    pvalue = pv/100.0
    f = open("data/plots/graph_priority_ctime_q"+str(drones)+"-p" +
             str(int(pvalue*100)) + "_" + str(distrib) + ".tex", "w+")
    f.write(plot_file_1)
    f.write("title={Graph priority \\(ct \\; q=" +
            str(drones)+", B=15, p="+str(pvalue)+"\\) (" + distrib + ")},")
    f.write(plot_file_3)
    f.write("ylabel={ budget },")
    f.write(plot_file_4)

    whichcolor = 0
    for which_priority in ["ct_pmax", "ct_pmed", "ct_pmin"]:
        for alg in algs:
            f.write("\\addplot+["+color[whichcolor])
            whichcolor = (whichcolor + 1) % len(algs)
            f.write(plot_file_5)
            # time_graphs[str(int(float(p)*100))]["ct_pmax"][alg][str(n)].append(float(ct_pmax))
            for n_nodes in time_graphs[str(pv)][which_priority][alg].keys():
                arr = numpy.array(
                    time_graphs[str(pv)][which_priority][alg][n_nodes])
                avg = numpy.mean(arr, axis=0)
                std = numpy.std(arr, axis=0)
                f.write(str(n_nodes) +
                        " {:.2f} {:.2f}".format(avg, std)+"\n")
            f.write("};\\addlegendentry{"+which_priority+" "+alg+"};\n\n")

    f.write(plot_file_6)
    f.close()
    plots.append(f.name)

#############################
## CREATION MAIN DOCUMENT ###
#############################

simfile = open("data/plots/simulations_data_" + distrib + ".tex", "w+")
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

#############################
####### TABLES ##############
#############################

for alg in algs:
    # f = open("data/plots/table_cycles_q2_prim.tex", "w+")
    simfile.write(part_file_table_1)
    for n in d_table_n_cycles[alg]:
        simfile.write("{} & ".format(n))
        pzero = 0
        for p in d_table_n_cycles[alg][n]:
            arr = numpy.array(d_table_n_cycles[alg][n][p])
            avg = numpy.mean(arr, axis=0)
            if p == '0':
                pzero = avg
                simfile.write("{:.2f} ".format(avg))
            else:
                formula = pzero + float(p)/100.0 * \
                    ((1.0*(float(n) + pzero))/16.0)
                simfile.write("& {:.2f} & {:.2f} ".format(avg, formula))
        simfile.write("\\\\ \n")
    simfile.write(part_file_table_2)
    simfile.write(
        "\\caption{Total number of cycles (" + distrib + ") \\(q=2\\), PRIM}")
    simfile.write(part_file_table_3)

for q in d_table_time_cycles_avg.keys():
    # f = open("data/plots/table_time_cycles_q" +
    #  str(q) + "_" + str(distrib) + ".tex", "w+")
    simfile.write(table_time_in_cycle_part_1)
    for n in d_table_time_cycles_avg[q].keys():
        simfile.write("\\multicolumn{1}{l|}{"+str(n)+"}")
        for alg in d_table_time_cycles_avg[q][n].keys():
            arr = numpy.array(d_table_time_cycles_avg[q][n][alg])
            avg = numpy.mean(arr, axis=0)
            arr = numpy.array(d_table_time_cycles_min[q][n][alg])
            _min = numpy.mean(arr, axis=0)
            simfile.write("& {:.2f} & {:.2f}".format(avg, _min))
        simfile.write("\\\\ \n")
    simfile.write(part_file_table_2)
    simfile.write(
        "\\caption{Budget per round \\(q=" + str(q) + "\\), (" + distrib + "), PRIM}")
    simfile.write(part_file_table_3)
    # f.close()
    # plots.append(f.name)

simfile.write(document_part_2)
simfile.close()
