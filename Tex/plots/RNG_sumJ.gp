set terminal postscript eps enhanced color lw 2 30
set output 'RNG_sumJ.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic < {/Symbol S}_{<ij>} J_{ij} >}'

plot "data/RNG_sumJ_L16.dat" w ye title "L = 16" pt 6,\
     "data/RNG_sumJ_L32.dat" w ye title "L = 32" pt 6
