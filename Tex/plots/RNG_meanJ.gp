set terminal postscript eps enhanced color lw 2 30
set output 'RNG_meanJ.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic <J>}'

plot "data/RNG_meanJ_L16.dat" w ye title "L = 16" pt 6,\
     "data/RNG_meanJ_L32.dat" w ye title "L = 32" pt 6
