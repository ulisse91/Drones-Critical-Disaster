import numpy
import sys
from collections import defaultdict

from texstrings import *

distrib = sys.argv[1]

algs = ["top", "prim", "gmax", "top-prim", "top-greedy"]
color = {"top": "cyan", "prim" : "blue", "gmax" : "black", "top-prim" : "red", "top-greedy" : "green" }

d_table_n_cycles = defaultdict(lambda: defaultdict(list))
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

plots = []

for alg in algs:
    with open("data/output/"+alg + "_" + str(distrib) + ".csv") as fIn:
        for line in fIn:
            n, b, q, p, s, fun_1, fun_2, fun_3, ct_max, ct_med, ct_min, time, n_cycles, avg_time_cycles, min_time_cycle = line.strip().split(",")

            d_fun_1[str(int(float(p)*100))][alg][int(q)
                                                 ][int(n)].append(float(fun_1.strip()))
            d_fun_2[str(int(float(p)*100))][alg][int(q)
                                                 ][int(n)].append(float(fun_2.strip()))
            d_fun_3[str(int(float(p)*100))][alg][int(q)
                                                 ][int(n)].append(float(fun_3.strip()))
            plot_time[str(int(float(p)*100))][alg][int(q)][int(n)
                                                           ].append(float(time.strip())/(float(n_cycles)/float(q)))

            if float(p) == 0:
                d_table_time_cycles_avg[int(q)][int(n)][alg].append(
                    float(avg_time_cycles.strip()))
                d_table_time_cycles_min[int(q)][int(n)][alg].append(
                    float(min_time_cycle.strip()))

            if alg == 'prim' and int(q) == 2:
                d_table_n_cycles[int(n)][str(p)].append(float(n_cycles))

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
                str(pvalue)+"\\) (" + distrib +")},")
        f2.write("title={\\(q="+str(drones)+", B=15, p=" +
                 str(pvalue)+"\\) (" + distrib +")},")
        f.write(plot_file_3)
        f2.write(plot_file_3)
        if pvalue == 0:
            f.write("ylabel={\\(wL^I\\)},")
        else:
            f.write("ylabel={\\(wL^{II}\\)},")
        f2.write("ylabel={\\(ct\\)},")
        f.write(plot_file_4)
        f2.write(plot_file_4)

        for alg in algs:
            f.write("\\addplot+["+color[alg])
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
            f2.write("\\addplot+["+color[alg])
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

for pv in range(0, 125, 25):
    pvalue = pv/100.0
    for drones in range(2, 8, 2):
        f = open("data/plots/time_q"+str(drones)+"-p" +
                 str(int(pvalue*100)) + "_" + str(distrib) + ".tex", "w+")
        f.write(plot_file_1)
        f.write("title={Execution time (average per round) \\(q=" +
                str(drones)+", B=15, p="+str(pvalue)+"\\) (" + distrib +")},")
        f.write(plot_file_3)
        f.write("ylabel={\\(\\mu s\\)},")
        f.write(plot_file_4)

        for alg in algs:
            f.write("\\addplot+["+color[alg])
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

simfile = open("data/plots/simulations_data_" + distrib +".tex", "w+")
simfile.write(document_part_1)
i = 1
plots.sort()
simfile.write(start_figure)
for plot in plots:
    simfile.write(
        "\\subfloat[]{\n\\includestandalone[scale=1.2]{" + plot.replace("data/", "") + "}\n}\n")
    if i % 2 == 0:
        simfile.write(end_figure)
        simfile.write(start_figure)
    i += 1
simfile.write(end_figure)

# f = open("data/plots/table_cycles_q2_prim.tex", "w+")
simfile.write(part_file_table_1)
for n in d_table_n_cycles:
    simfile.write("{} & ".format(n))
    pzero = 0
    for p in d_table_n_cycles[n]:
        arr = numpy.array(d_table_n_cycles[n][p])
        avg = numpy.mean(arr, axis=0)
        if p == '0':
            pzero = avg
            simfile.write("{:.2f} ".format(avg))
        else:
            formula = pzero + float(p)/100.0 * ((1.0*(float(n) + pzero))/16.0)
            simfile.write("& {:.2f} & {:.2f} ".format(avg, formula))
    simfile.write("\\\\ \n")
simfile.write(part_file_table_2)
simfile.write("\\caption{Total number of cycles (" + distrib +") \\(q=2\\), PRIM}")
simfile.write(part_file_table_3)
# f.close()

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
    simfile.write("\\caption{Budget per round, (" + distrib +"), PRIM}")
    simfile.write(part_file_table_3)
    # f.close()
    # plots.append(f.name)

simfile.write(document_part_2)
simfile.close()
