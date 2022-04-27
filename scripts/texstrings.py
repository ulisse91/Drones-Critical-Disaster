color = ["cyan", "blue", "black", "red", "green", "purple", "magenta", "yellow", "gray",
         "darkgray", "lightgray", "brown", "lime", "olive", "orange", "pink", "teal", "violet"]

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

plot_file_1v2 = """
\\documentclass{standalone}
\\usepackage{tikz}
\\usepackage{pgfplots}

\\pgfplotsset{width=7cm, compat=1.15}\n

\\begin{document}
\\begin{tikzpicture}
\\begin{axis}[
    xlabel={\\(|I|\\)},
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
    xtick={50,75,100,125,150,175,200},
    y label style={at={(-0.15,0.5)}},
    x label style={at={(0.5,-0.075)}},
    yticklabel style = {font=\\tiny},
    xticklabel style = {font=\\tiny}
]
"""

plot_file_4v2 = """
    every axis plot/.append style={thick},
    xmajorgrids, ymajorgrids,
    width=5.5cm, height=5.5cm,
    xtick={10,20,30,40,50,75,100,125,150,175,200},
    y label style={at={(-0.15,0.5)}},
    x label style={at={(0.5,-0.075)}},
    yticklabel style = {font=\\tiny},
    xticklabel style = {font=\\tiny},
    xticklabels={10,,30,,50,75,100,125,150,175,200}
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
"""

table_time_in_cycle_part_1half = """\\\\
\\multicolumn{1}{l|}{n}  & avg         & min         & avg         & min        & avg         & min         & avg & min \\\\
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
