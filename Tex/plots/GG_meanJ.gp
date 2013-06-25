set terminal postscript eps enhanced color blacktext lw 2 solid 26
set output 'GG_meanJ.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic <J>}'

set size square

plot "data/GG_meanJ_L16.dat" w ye title "L = 16" pt 4 ps 2,\
     "data/GG_meanJ_L32.dat" w ye title "L = 32" pt 6 ps 2
