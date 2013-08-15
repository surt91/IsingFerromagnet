set terminal postscript eps enhanced color blacktext lw 2 solid 26
set output 'autocorrT.eps'

set xlabel '{/Italic T}'
set ylabel '{/Italic {/Symbol t}}'

set xr [0.1:3.3]

set size square
set key samplen 1. left Left

set arrow from 2.2691,0 to 2.2691,14 nohead
set label 1 "{/Italic T_c}" at graph 0.60,0.50 left rotate by 90

plot 'data/Autokorrelationszeit_0.0.dat' using 1:2:3 w yerrorbar pt 4 title 'L = 16',\
     'data/Autokorrelationszeit_0.0.dat' using 1:4:5 w yerrorbar pt 6 title 'L = 32',\
     'data/Autokorrelationszeit_0.0.dat' using 1:6:7 w yerrorbar pt 8 title 'L = 64',\
     'data/Autokorrelationszeit_0.0.dat' using 1:8:9 w yerrorbar pt 10 title 'L = 128',\
     'data/Autokorrelationszeit_0.0.dat' using 1:10:11 w yerrorbar pt 12 title 'L = 256'
