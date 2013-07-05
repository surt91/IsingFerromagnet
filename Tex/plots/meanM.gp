set terminal postscript eps enhanced color blacktext lw 2 26
set output 'meanM.eps'

set xlabel '{/Italic T}'
set ylabel '{/Italic <|m|>}'
set xrange [1.5:3.2]

set size square

set key right

plot 'data/Mean_M_0_00.dat' using 1:2:3 w yerrorbar title '{/Italic L} = 16',\
     'data/Mean_M_0_00.dat' using 1:4:5 w yerrorbar title '{/Italic L} = 32',\
     'data/Mean_M_0_00.dat' using 1:6:7 w yerrorbar title '{/Italic L} = 64',\
     'data/Mean_M_0_00.dat' using 1:8:9 w yerrorbar title '{/Italic L} = 128'
