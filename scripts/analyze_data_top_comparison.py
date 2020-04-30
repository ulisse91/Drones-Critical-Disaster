import numpy
import sys
import os
from collections import defaultdict

data_fun_1_apx = defaultdict(lambda: defaultdict(list))
data_fun_2_apx = defaultdict(lambda: defaultdict(list))
data_fun_3_apx = defaultdict(lambda: defaultdict(list))

data_fun_1_heur = defaultdict(lambda: defaultdict(list))
data_fun_2_heur = defaultdict(lambda: defaultdict(list))
data_fun_3_heur = defaultdict(lambda: defaultdict(list))

with open("data/output/confronto_TOP_uniform.csv") as fIn:
    for line in fIn:
        n, b, q, p, s, fun_1_apx, fun_2_apx, fun_3_apx, fun_1_heur, fun_2_heur, fun_3_heur = line.strip().split(",")

        if int(q) == 2:
            data_fun_1_apx[int(n)][str(int(float(p)*100))].append(float(fun_1_apx))
            data_fun_2_apx[int(n)][str(int(float(p)*100))].append(float(fun_2_apx))
            data_fun_3_apx[int(n)][str(int(float(p)*100))].append(float(fun_3_apx))

            data_fun_1_heur[int(n)][str(int(float(p)*100))
                                    ].append(float(fun_1_heur))
            data_fun_2_heur[int(n)][str(int(float(p)*100))
                                    ].append(float(fun_2_heur))
            data_fun_3_heur[int(n)][str(int(float(p)*100))
                                    ].append(float(fun_3_heur))

# print(data_fun_1_apx)

for n in data_fun_1_apx.keys():
    for p in data_fun_1_apx[n].keys():
        for x in range(0,len(data_fun_1_apx[n][p])):
            # print(data_fun_1_apx[n][p][x], data_fun_1_heur[n][p][x], end=' ')
            data_fun_1_apx[n][p][x] = abs(data_fun_1_apx[n][p][x] - data_fun_1_heur[n][p][x])/float(data_fun_1_apx[n][p][x])
            # print(data_fun_1_apx[n][p][x])
        arr = numpy.array(data_fun_1_apx[n][p])
        avg = numpy.mean(arr, axis=0)
        std = numpy.std(arr, axis=0)
        print(n, avg, std)

print()

for n in data_fun_2_apx.keys():
    for p in data_fun_2_apx[n].keys():
        for x in range(0,len(data_fun_2_apx[n][p])):
            # print(data_fun_2_apx[n][p][x], data_fun_2_heur[n][p][x], end=' ')
            data_fun_2_apx[n][p][x] = abs(data_fun_2_apx[n][p][x] - data_fun_2_heur[n][p][x])/float(data_fun_2_apx[n][p][x])
            # print(data_fun_2_apx[n][p][x])
        arr = numpy.array(data_fun_2_apx[n][p])
        avg = numpy.mean(arr, axis=0)
        std = numpy.std(arr, axis=0)
        print(n, avg, std)

print()

for n in data_fun_3_apx.keys():
    for p in data_fun_3_apx[n].keys():
        for x in range(0,len(data_fun_3_apx[n][p])):
            # print(data_fun_3_apx[n][p][x], data_fun_3_heur[n][p][x], end=' ')
            data_fun_3_apx[n][p][x] = abs(data_fun_3_apx[n][p][x] - data_fun_3_heur[n][p][x])/float(data_fun_3_apx[n][p][x])
            # print(data_fun_3_apx[n][p][x])
        arr = numpy.array(data_fun_3_apx[n][p])
        avg = numpy.mean(arr, axis=0)
        std = numpy.std(arr, axis=0)
        print(n, avg, std)