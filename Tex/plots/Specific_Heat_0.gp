set terminal postscript eps enhanced color blacktext lw 2 26
set output 'Specific_Heat_0.eps'

set xlabel '{/Italic T}'
set ylabel '{/Italic c}'
set xrange [2.1:2.4]
set size square

set key samplen 1. left Left

plot 'data/Specific_Heat_0.dat' using 1:2:3 w ye lc 1 pt 4 title '{/Italic L} = 16',\
     'data/Specific_Heat_0.dat' using 1:2 w l lt 3 lc 1 notitle smooth csplines,\
     'data/Specific_Heat_0.dat' using 1:4:5 w ye lc 2 pt 6 title '{/Italic L} = 32',\
     'data/Specific_Heat_0.dat' using 1:4 w l lt 3 lc 2 notitle smooth csplines,\
     'data/Specific_Heat_0.dat' using 1:6:7 w ye lc 3 pt 8 title '{/Italic L} = 64',\
     'data/Specific_Heat_0.dat' using 1:6 w l lt 3 lc 3 notitle smooth csplines,\
     'data/Specific_Heat_0.dat' using 1:8:9 w ye lc 4 pt 10 title '{/Italic L} = 128',\
     'data/Specific_Heat_0.dat' using 1:8 w l lt 3 lc 4 notitle smooth csplines
