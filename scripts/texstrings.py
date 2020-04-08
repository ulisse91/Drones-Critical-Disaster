part_file_1 = """
\\documentclass{standalone}
\\usepackage{tikz}
\\usepackage{pgfplots}
\\usepackage{filecontents}

\\pgfplotsset{width=7cm, compat=1.15}\n
"""

part_file_2 = """
\\begin{document}
\\begin{tikzpicture}
\\begin{axis}[
    xlabel={\\(|V|\\)},
"""

part_file_3 = """
legend style={
        legend pos=outer north east,
        legend style={font=\\tiny},
    },
"""

part_file_4 = """
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
\\addlegendentry{\\(G_{max}\\)}

\\addplot+[red, mark=none, mark options={scale=0.75}, smooth, error bars/.cd, y fixed,	y dir=both, y explicit]
    table[x index=0, y index=1, y error index=2] {topprim.data};
\\addlegendentry{\\(TP\\)}

\\end{axis}
\\end{tikzpicture}
\\end{document}
"""

part_file_table_1 = """
\\begin{table}[]
\\begin{tabular}{l|l|l|l|l|l|l|l|l|l|}
n  & p=0 &  & p=0.25 &  & p=0.5 &  & p=0.75 &  & p=1 \\ \\hline
"""

part_file_table_2 = """
\\end{tabular}
\\end{table}
"""
