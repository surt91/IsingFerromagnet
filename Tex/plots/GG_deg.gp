set terminal postscript eps enhanced color blacktext lw 2 solid 26
set output 'GG_deg.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic K}'

set xr [-0.1:1.3]
set yr [3.9:5.2]

set size square

plot "data/GG_deg_L16.dat" w ye title "L = 16" pt 4 ps 2,\
     "data/GG_deg_L32.dat" w ye title "L = 32" pt 6 ps 2
