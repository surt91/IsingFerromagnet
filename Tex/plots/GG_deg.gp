set terminal postscript eps enhanced color lw 2 30
set output 'GG_deg.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel 'Degree'

plot "data/GG_deg_L16.dat" w ye title "L = 16" pt 6,\
     "data/GG_deg_L32.dat" w ye title "L = 32" pt 6
