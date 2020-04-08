import numpy
from collections import defaultdict

from texstrings import *

algs = ["top", "prim", "gmax", "top-prim"]

d_table_n_cycles = defaultdict(lambda: defaultdict(list))
d_fun_1 = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))
d_fun_2 = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))
d_fun_3 = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))

for alg in algs:
    with open("data/output/"+alg+".csv") as fIn:
        for line in fIn:
            n, b, q, p, s, fun_1, fun_2, fun_3, ct_max, ct_med, ct_min, time, n_cycles, avg_time_cycles, min_time_cycle = line.strip().split(",")

            d_fun_1[alg][int(q)][int(n)].append(float(fun_1.strip()))
            d_fun_2[alg][int(q)][int(n)].append(float(fun_2.strip()))
            d_fun_3[alg][int(q)][int(n)].append(float(fun_3.strip()))

            if alg == 'prim' and int(q) == 2:
                d_table_n_cycles[int(n)][str(p)].append(float(n_cycles))

for pv in range(0, 100, 25):
    pvalue = pv/100.0
    for drones in range(2, 8, 2):
        f = open("data/plots/q"+str(drones)+"-p" +
                 str(int(pvalue*100))+".tex", "w+")
        f2 = open("data/plots/ct_q"+str(drones)+"-p" +
                  str(int(pvalue*100))+".tex", "w+")
        f.write(part_file_1)
        f2.write(part_file_1)
        for alg in algs:
            f.write("\\begin{filecontents}{"+alg+".data}\n")
            if pvalue == 0:
                for n_nodes in d_fun_1[alg][drones].keys():
                    arr = numpy.array(d_fun_1[alg][drones][n_nodes])
                    avg = numpy.mean(arr, axis=0)
                    std = numpy.std(arr, axis=0)
                    f.write(str(n_nodes) +
                            " {:.2f} {:.2f}".format(avg, std)+"\n")
                f.write("\\end{filecontents}\n\n")
            else:
                for n_nodes in d_fun_2[alg][drones].keys():
                    arr = numpy.array(d_fun_2[alg][drones][n_nodes])
                    avg = numpy.mean(arr, axis=0)
                    std = numpy.std(arr, axis=0)
                    f.write(str(n_nodes) +
                            " {:.2f} {:.2f}".format(avg, std)+"\n")
                f.write("\\end{filecontents}\n\n")
            f2.write("\\begin{filecontents}{"+alg+".data}\n")
            for n_nodes in d_fun_3[alg][drones].keys():
                arr = numpy.array(d_fun_3[alg][drones][n_nodes])
                avg = numpy.mean(arr, axis=0)
                std = numpy.std(arr, axis=0)
                f2.write(str(n_nodes)+" {:.2f} {:.2f}".format(avg, std)+"\n")
            f2.write("\\end{filecontents}\n\n")
        f.write(part_file_2)
        f2.write(part_file_2)
        f.write("title={\\(q="+str(drones)+", B=15, p="+str(pvalue)+"\\)},")
        f2.write("title={\\(q="+str(drones)+", B=15, p="+str(pvalue)+"\\)},")
        f.write(part_file_3)
        f2.write(part_file_3)
        if pvalue == 0:
            f.write("ylabel={\\(wL^I\\)},")
        else:
            f.write("ylabel={\\(wL^{II}\\)},")
        f2.write("ylabel={\\(ct\\)},")
        f.write(part_file_4)
        f.write(part_file_4)
        f2.write(part_file_4)
        f.close()
        f2.close()


f = open("data/plots/table_cycles_q2_prim.tex", "w+")
f.write(part_file_table_1)
for n in d_table_n_cycles:
    f.write("{} & ".format(n))
    for p in d_table_n_cycles[n]:
        arr = numpy.array(d_table_n_cycles[n][p])
        avg = numpy.mean(arr, axis=0)
        std = numpy.std(arr, axis=0)
        if p == '0':
            f.write("{:.2f} ".format(avg))
        else:
            f.write("& & {:.2f} ".format(avg))
    f.write("\n")
f.write(part_file_table_2)
f.close()
