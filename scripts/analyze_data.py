import numpy
import sys
from collections import defaultdict

algs = ["top", "prim", "gmax", "topprim"]
pvalue = float(sys.argv[1])

start_file = """
\\documentclass{standalone}
\\usepackage{tikz}
\\usepackage{pgfplots}
\\usepackage{filecontents}

\pgfplotsset{width=7cm, compat=1.15}\n
"""

end_file_1 = """
\\begin{document}
\\begin{tikzpicture}
\\begin{axis}[
    xlabel={\(|V|\)},
    ylabel={\(wSOL^I\)},
    legend style={
        legend pos=outer north east,
        legend style={font=\tiny},
    },
"""


end_file_2 ="""
    every axis plot/.append style={thick},
    xmajorgrids, ymajorgrids,
    width=5.5cm, height=5.5cm,
    xtick={30,40,50,60,70,80,90,100},
    y label style={at={(-0.15,0.5)}},
    x label style={at={(0.5,-0.075)}}
]

\\addplot+[cyan, dashed, mark=none, mark options={scale=0.75}, smooth, error bars/.cd, y fixed,	y dir=both, y explicit]
    table[x index=0, y index=1, y error index=2] {top.data};
\\addlegendentry{TOP}

\\addplot+[blue, mark=none, mark options={scale=0.75}, smooth, error bars/.cd, y fixed,	y dir=both, y explicit]
    table[x index=0, y index=1, y error index=2]
    {prim.data};
\\addlegendentry{PRIM}

\\addplot+[black, mark=none, mark options={scale=0.75}, smooth, error bars/.cd, y fixed,	y dir=both, y explicit]
    table[x index=0, y index=1, y error index=2] {gmax.data};
\\addlegendentry{\(G_{max}\)}

\\addplot+[orange, dashed, mark=none, mark options={scale=0.75}, smooth, error bars/.cd, y fixed, y dir=both, y explicit]
    table[x index=0, y index=1, y error index=2] {gmin.data};
\\addlegendentry{\(G_{min}\)}

\\addplot+[red, mark=none, mark options={scale=0.75}, smooth, error bars/.cd, y fixed,	y dir=both, y explicit]
    table[x index=0, y index=1, y error index=2] {topprim.data};
\\addlegendentry{\(TP\)}

\end{axis}
\end{tikzpicture}
\end{document}
"""

for drones in range(2, 8, 2):
    f = open("data/plots/q"+str(drones)+"-p"+str(int(pvalue*100))+".tex", "w+")
    f2 = open("data/plots/ct_q"+str(drones)+"-p" +
              str(int(pvalue*100))+".tex", "w+")
    f.write(start_file)
    f2.write(start_file)
    for alg in algs:
        d_fun_1 = defaultdict(lambda: defaultdict(list))
        d_fun_2 = defaultdict(lambda: defaultdict(list))
        d_fun_3 = defaultdict(lambda: defaultdict(list))
        with open("data/output/"+alg+".csv") as fIn:
            for line in fIn:
                n, b, q, p, s, fun_1, fun_2, fun_3, ct_max, ct_med, ct_min, time = line.strip().split(",")
                d_fun_1[int(q)][int(n)].append(float(fun_1.strip()))
                d_fun_2[int(q)][int(n)].append(float(fun_2.strip()))
                d_fun_3[int(q)][int(n)].append(float(fun_3.strip()))
        f.write("\\begin{filecontents}{"+alg+".data}\n")
        if pvalue == 0:
            for n_nodes in d_fun_1[drones].keys():
                arr = numpy.array(d_fun_1[drones][n_nodes])
                avg = numpy.mean(arr, axis=0)
                std = numpy.std(arr, axis=0)
                f.write(str(n_nodes)+" {:.2f} {:.2f}".format(avg, std)+"\n")
            f.write("\\end{filecontents}\n\n")
        else:
            for n_nodes in d_fun_2[drones].keys():
                arr = numpy.array(d_fun_2[drones][n_nodes])
                avg = numpy.mean(arr, axis=0)
                std = numpy.std(arr, axis=0)
                f.write(str(n_nodes)+" {:.2f} {:.2f}".format(avg, std)+"\n")
            f.write("\\end{filecontents}\n\n")
        f2.write("\\begin{filecontents}{"+alg+".data}\n")
        for n_nodes in d_fun_3[drones].keys():
            arr = numpy.array(d_fun_3[drones][n_nodes])
            avg = numpy.mean(arr, axis=0)
            std = numpy.std(arr, axis=0)
            f2.write(str(n_nodes)+" {:.2f} {:.2f}".format(avg, std)+"\n")
        f2.write("\\end{filecontents}\n\n")
    f.write(end_file_1)
    f2.write(end_file_1)
    f.write("title={\\(q="+str(drones)+", B=15, p="+str(pvalue)+"\\)},")
    f2.write("title={\\(q="+str(drones)+", B=15, p="+str(pvalue)+"\\)},")
    f.write(end_file_2)
    f2.write(end_file_2)
