set terminal postscript eps enhanced color lw 2 20
set output 'GG_meanJ.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic <J>}'

plot "data/GG_meanJ_L16.dat" w ye title "L = 16" pt 6,\
     "data/GG_meanJ_L32.dat" w ye title "L = 32" pt 6
