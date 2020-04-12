plot_file_1 = """
\\documentclass{standalone}
\\usepackage{tikz}
\\usepackage{pgfplots}

\\pgfplotsset{width=7cm, compat=1.15}\n

\\begin{document}
\\begin{tikzpicture}
\\begin{axis}[
    xlabel={\\(|V|\\)},
"""

plot_file_3 = """
legend style={
        legend pos=outer north east,
        legend style={font=\\tiny},
    },
"""

plot_file_4 = """
    every axis plot/.append style={thick},
    xmajorgrids, ymajorgrids,
    width=5.5cm, height=5.5cm,
    xtick={10,20,30,40,50,60,70,80,90,100},
    y label style={at={(-0.15,0.5)}},
    x label style={at={(0.5,-0.075)}},
    yticklabel style = {font=\\tiny,xshift=0.5ex},
    xticklabel style = {font=\\tiny,xshift=0.5ex}
]
"""

plot_file_5 = """, mark=none, mark options={scale=0.75}, smooth, error bars/.cd, y fixed,	y dir=both, y explicit] table [x index=0, y index=1, y error index=2] {
"""

plot_file_6 = """
\\end{axis}
\\end{tikzpicture}
\\end{document}
"""

part_file_table_1 = """
\\begin{table}[ht]
\\centering
\\begin{tabular}{l|l|l|l|l|l|l|l|l|l|}
n  & p=0 &  p=0.25 &  & p=0.5 &  & p=0.75 &  & p=1 & \\\\ \\hline
"""

part_file_table_2 = """
\\end{tabular}
"""

part_file_table_3 = """
\\end{table}
"""

table_time_in_cycle_part_1 = """
\\begin{table}[ht]
\\centering
\\begin{tabular}{lll|ll|ll|ll|ll}
                        & \\multicolumn{2}{c|}{PRIM} & \\multicolumn{2}{c|}{TOP} & \\multicolumn{2}{c|}{GMAX} & \\multicolumn{2}{c|}{T+P} & \\multicolumn{2}{c}{T+G} \\\\
\\multicolumn{1}{l|}{n}  & avg         & min         & avg         & min        & avg         & min         & avg & min & avg & min \\\\
"""

document_part_1 = """
\\documentclass{article}
\\usepackage[margin=0.5in]{geometry}
\\usepackage[utf8]{inputenc}
\\usepackage[mode=buildnew]{standalone}
\\usepackage{tikz}
\\usepackage{pgfplots}
\\usepackage[caption=false]{subfig}

\\begin{document}
"""

document_part_2 = """
\\end{document}
"""

start_figure = """
\\begin{figure}[ht]
\\centering
"""

end_figure = """
\\caption{}
\\label{fig:experiments}
\\end{figure}
"""
